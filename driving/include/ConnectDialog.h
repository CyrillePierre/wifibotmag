/*
    ConnectDialog.h

		Interface permettant la connexion au WifiBot.
*/

#ifndef __CONNECTDIALOG_H__
#define __CONNECTDIALOG_H__

#include <QDialog>

class QWidget;
class QLabel;
class QPushButton;
class QComboBox;
class QGroupBox;
class QHBoxLayout;
class Controler;
class InterfaceVideo;
class ConfigModel;

class ConnectDialog : public QDialog
{
  Q_OBJECT

public:

  ConnectDialog(ConfigModel *configModel, Controler *controler, InterfaceVideo *interfaceVideo, QWidget *parent = 0);

private:

	// Numéro du wifiBot auquel on est connecté.
	int numWifiBot;

	QLabel *imgLabel;
  QLabel *buttonLabel;
  QLabel *titreLabel;
  QLabel *sousTitreLabel1;
  QLabel *sousTitreLabel2;
  QPushButton *connectButton;
	QPushButton *quitButton;
  QComboBox *wifiComboBox;
	QGroupBox *resultGroupBox;
  QLabel *execLabel1;
	QLabel *execLabel2;
	QLabel *execLabel3;
	QLabel *resultLabel1;
	QLabel *resultLabel2;

	// Controller du wifibot.
	Controler *controler;
	InterfaceVideo *interfaceVideo;

	// Configuration.
	ConfigModel *configModel;

	// Effaçage des labels d'information.
	void clearLabels(void);

signals:

	// La connexion à tous les périphériques à réussie.
	void isConnected(bool);

private slots:
	
	// Appuie sur le boutton de connexion.
	void PushConnectButton(void);

public slots:

	// Initialisation de la boite de dialogue de connexion à l'ouverture.
	void InitConnectDialog(void);
	// La configuration a été changée.
	void SavedConfig(void);

};

#endif
