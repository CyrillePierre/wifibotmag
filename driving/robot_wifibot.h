#ifndef ROBOT_WIFIBOT_H
#define ROBOT_WIFIBOT_H

#include <QtGui>
#include <QtGui/QMainWindow>
#include "InterfaceVideo.h"
#include "ConnectDialog.h"
#include "ConfigDialog.h"
#include "Configmodel.h"
#include "Joystick.h"
#include "Keyboard.h"
#include "Controler.h"

class Robot_Wifibot : public QMainWindow
{
	Q_OBJECT

public:
	Robot_Wifibot(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Robot_Wifibot();

private:

	void createActions();
    void createMenus();

	// Elements du menu.
	QMenu *fileMenu;
	QAction *QuitterAct;

	QMenu *wifibotMenu;
	QAction *ConnectAct;
	QAction *DisconnectAct;

	QMenu *commandsMenu;
	QActionGroup *CommandGrp;
	QAction *JoystickAct;
	QAction *KeyboardAct;
	QAction *ConfigAct;

	QMenu *aideMenu;
	QAction *HowAct;
	QAction *AboutAct;
	QAction *AboutQtAct;

    // Indicateur du niveau de la baterie.
	QProgressBar *bateryLevel;
	// Indicateur de la vitesse des roues.
	QLCDNumber *speedWheelUpRight;
	QLCDNumber *speedWheelUpLeft;
	QLCDNumber *speedWheelDownRight;
	QLCDNumber *speedWheelDownLeft;
	// Indicateur des capteurs infrarouges.
	QProgressBar *rightInfraredIndicator;
	QProgressBar *leftInfraredIndicator;

	// Bar de statut.
	QStatusBar *statusBar;

    // Interface pour affichage video.
	InterfaceVideo *interfaceVideo;

	// Controleur du robot.
	Controler *controler;

	// Interface de connexion au Wifibot et à la WebCam.
	ConnectDialog *connectDialog;

	// Interface de configuration des media de commande du robot.
	ConfigDialog *configDialog;

	// Modèle qui contient la configuration des media.
	ConfigModel *configModel;

	// Interface de commande.
	AbstractCommand *abstractCommand;

	// Différentes commandes possibles.
	Keyboard *keyboard;
	Joystick *joystick;

private slots:
	
	// Connexion du robot.
	void Connect(bool);
	// Déconnexion au robot.
	void Disconnect(void);

	// Choix du Joystick en controle.
	void InJoystick(void);
	// Choix du clavier.
	void InKeyboard(void);

	// Changement de commande.
	void setCommand(AbstractCommand *, const QString &);

	// Mise à jour de la bar de status.
	void updateStatusBar(const QString&);

	// Aide.
	void HowToDo(void);
	// Information sur la version de Qt utilisée.
	void AboutQt(void);
	// Information sur le logiciel.
	void About(void);
};

#endif // ROBOT_WIFIBOT_H
