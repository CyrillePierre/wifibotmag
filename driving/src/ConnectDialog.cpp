/*
    ConnectDialog.cpp
*/

#include <QFont>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QPixmap>
#include <QGroupBox>
#include <QHBoxLayout>

#include "ConnectDialog.h"
#include "Controler.h"
#include "InterfaceVideo.h"
#include "Configmodel.h"

ConnectDialog::ConnectDialog(ConfigModel *configModel, Controler *controler, InterfaceVideo *interfaceVideo, QWidget *parent)
	: QDialog(parent), configModel(configModel)
{
	this->setWindowModality(Qt::WindowModal);
	this->resize(420, 350);
	this->setMinimumSize(QSize(420, 350));
	this->setMaximumSize(QSize(420, 350));
	this->setWindowTitle(tr("Connexion"));

	imgLabel = new QLabel(this);
	imgLabel->setGeometry(QRect(0, 0, 130, 300));
	imgLabel->setPixmap(QPixmap(":/images/ill_connexion.png"));

	buttonLabel = new QLabel(this);
	buttonLabel->setGeometry(QRect(0, 300, 420, 50));
	buttonLabel->setFrameShape(QFrame::Panel);
	buttonLabel->setFrameShadow(QFrame::Raised);

	titreLabel = new QLabel(this);
	titreLabel->setGeometry(QRect(150, 20, 181, 21));
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setWeight(75);
	titreLabel->setFont(font);
	titreLabel->setText(tr("Connexion au WifiBot"));

	sousTitreLabel1 = new QLabel(this);
	sousTitreLabel1->setGeometry(QRect(150, 50, 231, 20));
	sousTitreLabel1->setText(tr("Choisissez le WifiBot auquel vous voulez vous"));

	sousTitreLabel2 = new QLabel(this);
	sousTitreLabel2->setGeometry(QRect(150, 70, 81, 16));
	sousTitreLabel2->setText(tr("connecter."));

	connectButton = new QPushButton(buttonLabel);
	connectButton->setGeometry(QRect(230, 12, 90, 25));
	connectButton->setText(tr("Connexion"));

	quitButton = new QPushButton(buttonLabel);
	quitButton->setGeometry(QRect(330, 12, 70, 25));
	quitButton->setText(tr("Quitter"));

	wifiComboBox = new QComboBox(this);
	wifiComboBox->setGeometry(QRect(260, 70, 100, 22));
	
	resultGroupBox = new QGroupBox(tr("Résultats"), this);
	resultGroupBox->setGeometry(150, 130, 240, 110);
	
	QGridLayout *resultGridlayout = new QGridLayout;
	resultGridlayout->setColumnMinimumWidth(0, 180);

	execLabel1 = new QLabel;
	execLabel1->resize(240, 16);
	resultLabel1 = new QLabel;
	resultLabel1->resize(16, 16);
	execLabel2 = new QLabel;
	execLabel2->resize(240, 16);
	resultLabel2 = new QLabel;
	resultLabel2->resize(16, 16);
	execLabel3 = new QLabel;
	execLabel3->resize(240, 16);
	
	resultGridlayout->addWidget(execLabel1, 0, 0);
	resultGridlayout->addWidget(resultLabel1, 0, 1);
	resultGridlayout->addWidget(execLabel2, 1, 0);
	resultGridlayout->addWidget(resultLabel2, 1, 1);
	resultGridlayout->addWidget(execLabel3, 2, 0);
	resultGroupBox->setLayout(resultGridlayout);

	// Initialisation du controler et de l'interface video.
	this->controler = controler;
	this->interfaceVideo = interfaceVideo;

	// Appuie sur les bouttons.
	connect(connectButton, SIGNAL(released()), this, SLOT(PushConnectButton()));
	connect(quitButton, SIGNAL(released()), this, SLOT(close()));

	// Pour pouvoir mettre à jour les élément dépendant de la configuration.
	connect(configModel, SIGNAL(update()), this, SLOT(SavedConfig()));
}

void ConnectDialog::InitConnectDialog()
{
	// L'ouverture de cette boite de dialoque n'est possible que si l'on est déconnecté.

	// Effaçage des informations.
	this->clearLabels();
	// Autorisation de choisir un autre wifibot.
	wifiComboBox->setDisabled(false);
	// Autorisaion de se connecter.
	connectButton->setEnabled(true);

	// Affichage de la boite de dialogue.
	show();
}

void ConnectDialog::PushConnectButton()
{
	// Désactivation du bouton de connexion et de fermeture pendant le traitement.
	connectButton->setDisabled(true);
	connectButton->repaint();
	quitButton->setDisabled(true);
	quitButton->repaint();
	// Désactivation de la group box.
	wifiComboBox->setDisabled(true);
	wifiComboBox->repaint();
	
	// On efface les labels d'information.
	this->clearLabels();

	// Demande de connexion au WifiBot.
	execLabel1->setText(tr("Connexion au WifiBot..."));
	execLabel1->repaint();

	if (controler->connect_robot(configModel->getCommandAddress(wifiComboBox->currentText()), 15000))	 		
		resultLabel1->setPixmap(QPixmap(":/images/tick.png"));
	else 
		resultLabel1->setPixmap(QPixmap(":/images/cross.png"));
	resultLabel1->repaint();

	// Demande de connexion a la WebCam.
	execLabel2->setText(tr("Connexion a la WebCam..."));
	execLabel2->repaint();

    // TODO commnenté pour virer la cam
    if (interfaceVideo->connect_video(configModel->getWebcamAddress(wifiComboBox->currentText())))
        resultLabel2->setPixmap(QPixmap(":/images/tick.png"));
    else
        resultLabel2->setPixmap(QPixmap(":/images/cross.png"));
    resultLabel2->repaint();

	// Si la connexion a réussie.
    if (controler->isConnected() /*&& interfaceVideo->isConnected()*/)
	{
		execLabel3->setText(tr("Connexion réussie !"));
		quitButton->setText(tr("Continuer"));
		quitButton->setDisabled(false);
		emit isConnected(true);
	}
	else
	{
		// Déconnexion de tous les périph.
		controler->disconnect_robot();
		interfaceVideo->stop_video();

		execLabel3->setText(tr("Connexion échouée !"));
		// On peut choisir un autre robot.
		wifiComboBox->setDisabled(false);
		// Réactivation du boutton de connexion pour une autre tentative.
		connectButton->setDisabled(false);
		quitButton->setDisabled(false);
	}
}

void ConnectDialog::clearLabels()
{
	execLabel1->clear();
	execLabel1->repaint();
	execLabel2->clear();
	execLabel2->repaint();
	execLabel3->clear();
	execLabel3->repaint();
	resultLabel1->clear();
	resultLabel1->repaint();
	resultLabel2->clear();
	resultLabel2->repaint();
}

void ConnectDialog::SavedConfig()
{
	// Netoyage de la liste précédente.
	wifiComboBox->clear();
	// Mise à jour de la liste des wifibots.
	wifiComboBox->addItems(configModel->getWifibotList());
}
