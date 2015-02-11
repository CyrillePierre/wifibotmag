/*
    InterfaceVideo.cpp
*/

#include "Receiver_video.h"
#include "InterfaceVideo.h"

//#include <opencv/cv.h>
#include <QtGui>
#include <stdio.h>

//#pragma comment (lib,"cv")
//#pragma comment (lib,"cvaux")
//#pragma comment (lib,"cvcam")
//#pragma comment (lib,"cvhaartraining")
//#pragma comment (lib,"cxcore")
//#pragma comment (lib,"cxts")
//#pragma comment (lib,"highgui")
//#pragma comment (lib,"trs")

InterfaceVideo::InterfaceVideo(QWidget *parent, Controler * controle):QScrollArea(parent),controle(controle)
{
	imageLabel = new QLabel(this);
	receiverThread_video = new ReceiverThread_Video;

	/* Enregistrement du type QImage pour la transmission
     de QImage dans une connexion */
	 qRegisterMetaType<QImage>("QImage");

	// Connexion des slots et des signaux.
	connect(receiverThread_video, SIGNAL(receivedImage(const QImage &)),this, SLOT(updatePixmap(const QImage &)));

	pFichier.open("output.txt");

//	Xp=MatrixXd::Constant(2,1,0); //prediction precedente
//	X=MatrixXd::Constant(2,1,0); //prediction courante
//	Cx=MatrixXd::Identity(2,2); //erreur prediction-mesure == X² 0 // 0 Y²
//	Cy=MatrixXd::Identity(2,2); //erreur sur obeservation courante == u² 0 // 0 v²
//	Y=MatrixXd::Constant(2,1,0); //observation courante (=u,v) =X ?
//	K = MatrixXd::Constant(2,2,0); //gain de kalm&an

//	Cx(0,0) = 160.0;
//	Cx(1,1) = 120.0;
}

bool InterfaceVideo::connect_video(const QString &numWifiBot) 
{
	// On fixe une taille suffisante pour la video
	imageLabel->setFixedSize(this->size());

    // TODO à décommenter pour faire marcher la cam
	// Démarrage de la connexion.
//	if (receiverThread_video->startVideo(numWifiBot))
//		return true;
//	else
//		return false;
    return true;
}

void InterfaceVideo::updatePixmap(const QImage &image)
{
  img = image ;
  displayImage() ;
}



void InterfaceVideo::displayImage()
{
    // TODO à enlever pour afficher l'image de la cam
    return;

  QImage final ; //image QT affichée
//  IplImage image; //image source


  if( ! img.isNull()) 
	{    
    final = img ; 

	unsigned int width = final.width();
	unsigned int height = final.height();

	unsigned int taille_image = width*height;
	char * mask = new char [taille_image] ; //alocation du masque
	char * delmask = new char [taille_image] ; //alocation du masque de suppression
		

    /* Initialisation de l'entete */
//    cvInitImageHeader( &image, cvSize(width*4/3, height), 8, 3, IPL_ORIGIN_TL, 4);
    
    /* Pointe sur les données binaires de l'image */
//    cvSetData( &image, (char*) final.bits(), final.bytesPerLine() );

/*	//boucle de detection rouge
	for (unsigned int i=0; i<(taille_image*4);i=i+4)
	{
		unsigned char b,g,r; //composantes
		
		//recup valeurs RGB
		b=(unsigned char)image.imageData[i];
		g=(unsigned char)image.imageData[i+1];
		r=(unsigned char)image.imageData[i+2];
		
		//detection rouge
		if (b<0x8E && g<0x8E && r>0x96)
		{
			image.imageData[i]=0;
			image.imageData[i+1]=0;
			image.imageData[i+2]=0xFF;
			mask[i/4]=1; //pixel detecté
		}
		else mask[i/4]=0; //pixel non detecté
		
		delmask[i/4]=0; //init mask deletion
	}

		//filtrage du masque
	for (unsigned int iter=0;iter<5;++iter) //5 passes
	{
		for (unsigned int i = 0; i< taille_image ; ++i)
		{
			unsigned char flag_top=0,flag_bottom=0,flag_left=0,flag_right=0,flag_set=0;
			signed int ligneprec = i-width;
			signed int lignesuiv = i+width;
			char count = 0;
			unsigned char seuil = 4;
			unsigned char seuil_cote = seuil-2;
			unsigned char seuil_angle = seuil-3;
			unsigned char flag_del = 0;

			if (ligneprec < 0) //haut de l'image
			{
				flag_set=1;
				flag_top = 1;
			}
			if(lignesuiv > taille_image ) //bas de l'image
			{
				flag_bottom = 1;
				flag_set=1;
			}
			if (i%width == 0) //premiere colonne
			{
				flag_left=1;
				flag_set=1;
			}
			if (i%width == width-1) //derniere colonne
			{
				flag_right = 1;
				flag_set=1;
			}
		
			//trqitement si pixel activé
			if (mask[i]==1)
			{
				if (!flag_set) //traitement cas general
				{
					count = mask[i-1] + mask [i+1] + mask [ligneprec-1]+ mask [ligneprec]+ mask [ligneprec+1] + mask [lignesuiv-1]+ mask [lignesuiv]+ mask [lignesuiv+1];
					if (count<seuil) flag_del=1;
				}
				else if (flag_top) //cas pixel sur premiere ligne
				{
					if (!flag_left && !flag_right) //cqs general ligne haut
					{
						count = mask[i-1] + mask [i+1] + mask [lignesuiv-1]+ mask [lignesuiv]+ mask [lignesuiv+1];
						if (count<seuil_cote) flag_del=1;
					}
					else if (flag_left) //haut a gauche
					{
						count = mask [i+1] + mask [lignesuiv] + mask [lignesuiv+1];
						if (count<seuil_angle) flag_del=1;
					}
					else //flag right
					{
						count = mask[i-1] + mask [lignesuiv-1]+ mask [lignesuiv];
						if (count<seuil_angle) flag_del=1;
					}
				}
				else if (flag_bottom) //cas pixel sur premiere ligne
				{
					if (!flag_left && !flag_right) //cqs general ligne haut
					{
						count = mask[i-1] + mask [i+1] + mask [ligneprec-1]+ mask [ligneprec]+ mask [ligneprec+1];
						if (count<seuil_cote) flag_del=1;
					}
					else if (flag_left) //haut a gauche
					{
						count = mask [i+1] + mask [ligneprec] + mask [ligneprec+1];
						if (count<seuil_angle) flag_del=1;
					}
					else //flag right
					{
						count = mask[i-1] + mask [ligneprec-1]+ mask [ligneprec];
						if (count<seuil_angle) flag_del=1;
					}
				}
				else if (flag_left) //cas pixel sur premiere ligne
				{
					count = mask [i+1] + mask [ligneprec]+ mask [ligneprec+1] + mask [lignesuiv]+ mask [lignesuiv+1];
					if (count<seuil_cote) flag_del=1;
				}
				else //cote droit cqs general
				{
					count = mask[i-1] + mask [ligneprec-1]+ mask [ligneprec] + mask [lignesuiv-1]+ mask [lignesuiv];
					if (count<seuil_cote) flag_del=1;
				}
				if (flag_del)
				{
					delmask[i] = -1; //pixel supprimé
					mask[i]=0;//pixel supprimé
				}
			}
		}
	}

	//boucle de visualisation masque
	unsigned int first_pix = 0,last_pix=0;
	for (unsigned int i=0; i<(taille_image*4);i=i+4)
	{		
		if (mask[i/4]==1)
		{
			if (!first_pix) //pixel interessant
			{
				first_pix=i/4;
				image.imageData[i]=0xFF;
				image.imageData[i+1]=0x00;
				image.imageData[i+2]=0x00;
			}
			else last_pix=i/4;
		}
		
		//en vert : supprimé
		if (delmask[i/4]==-1)
		{
			image.imageData[i]=0;
			image.imageData[i+1]=0xFF;
			image.imageData[i+2]=0x00;
		}
	}

	if(last_pix) //coloration bleue
	{
		image.imageData[last_pix*4]=0xFF;
		image.imageData[last_pix*4+1]=0x00;
		image.imageData[last_pix*4+2]=0x00;
	}

	//cqlcul point milieu
	//cordonnées 1er point
	
	if (first_pix && last_pix) //si deux detections !
	{
		unsigned int x_first = first_pix % width;
		unsigned int y_first = first_pix / width;

		unsigned int x_last = last_pix % width;
		unsigned int y_last = last_pix / width;

		//cqlcul du centre
		int x_centre = (x_last+x_first)/2;
		int y_centre = (y_last+y_first)/2;

		//sauvegarde centre
		pFichier << x_centre << ' ' << y_centre << endl;

		unsigned int centre = y_centre * width + x_centre;

		image.imageData[centre*4]=0xFF;
		image.imageData[centre*4+1]=0x00;
		image.imageData[centre*4+2]=0x00;

		unsigned char slow = 2;
		unsigned char zone_neutre = 30;

		/*Xp=MatrixXd::Constant(2,1,0); //prediction precedente
		X=MatrixXd::Constant(2,1,0); //prediction courante
		Cx=MatrixXd::Constant(2,2,0); //erreur prediction-mesure == X² 0 // 0 Y²
		Cy=MatrixXd::Constant(2,2,0); //erreur sur obeservation courante == u² 0 // 0 v²
		Y=MatrixXd::Constant(2,1,0); //observation courante (=u,v) =X ?
		K = MatrixXd::Constant(2,2,0); //gain de kalm&an*/

/*		if (x_centre == 160)
		{
			x_centre ++;
		}
		if (y_centre == 120)
		{
			y_centre ++;
		}

		double x_kalman = (double) x_centre - 160.0;
		double y_kalman = (double) y_centre - 120.0;

		
		// Sauvegarde de X
		Xp = X;

		// Point du centre detecté => dans Y
		Y(0,0)=x_kalman;
		Y(1,0)=y_kalman;


		//calcul de la matrice de covariance actuelle
		Cx = Cx - (K*Cx);

		//cqlcul de Cy : erreur sur l'observation
		Cy(0,0) = x_kalman*x_kalman;
		Cy(1,1) = y_kalman*y_kalman;

		// Calcul du gain de Kalman
		K = (Cx + Cy);
		K= K.inverse();
		K= Cx * K;

		//calcul de l'état courant
		X = Xp + K*(Y-Xp);

		double X_kal=X(0,0);

		pFichier << Cx << endl << Cy << endl << endl;

		//commande corrigee pqr kalman
		int vitesse = (60+(X(0,0))/160*60);
		if (X_kal < 0.0) //lq cible est à gauche -- il faut tourner a gauche
		{
			controle->setSpeedRight(-vitesse/2);
			controle->setSpeedLeft(vitesse);
		}
		else if (X_kal > 0.0) // sinon bah logique quoi
		{
			controle->setSpeedLeft(-vitesse/2);
			controle->setSpeedRight(vitesse);
		}
		else
		{
			controle->setSpeedRight(60/slow);
			controle->setSpeedLeft(60/slow);
		}


		/******* OK sans Kalman
		if (x_centre < ((width/2)-zone_neutre)) //lq cible est à gauche -- il faut tourner a gauche
		{
			controle->setSpeedRight(40);
			controle->setSpeedLeft(-(60-((width/2)-x_centre)/160*60)/slow);
		}
		else if (x_centre > ((width/2)+zone_neutre)) // sinon bah logique quoi
		{
			controle->setSpeedLeft(40);
			controle->setSpeedRight(-(60-(x_centre-(width/2))/160*60)/slow);
		}
		else
		{
			controle->setSpeedRight(60/slow);
			controle->setSpeedLeft(60/slow);
		}*/
/*	}
	else //si pas de detection, on arrete
	{
		controle->setSpeedRight(0);
		controle->setSpeedLeft(0);
	}
	*/

	//calcul du point milieu
	//A FAIRE
	//calcul des coordonnees XY pour les deux points
	//determiner le point milieu
	//consignes commandes



	final=final.scaled(imageLabel->size());
    /* Affichage de l'image passée en parametre
       (envoyée par un signal du receveur) dans le label */
    QPixmap qPix = QPixmap::fromImage(final); 
   imageLabel->setPixmap(qPix);

    emit imageChange(qPix);

	delete [] mask; //liberation du masque
	delete [] delmask;
  }
}

bool InterfaceVideo::isConnected()
{
	return receiverThread_video->isConnected();
	return true;
}

void InterfaceVideo::stop_video()
{
	receiverThread_video->stopVideo();
	// supression de l'affichage de la dernière image
	imageLabel->clear();
}
