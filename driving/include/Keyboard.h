/*
    Keyboard.h
*/

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "AbstractCommand.h"
#include <QWidget>
#include <QString>

class Controler;
class ConfigModel;
class QObject;
class QKeyEvent;
class KeyboardCaptEvent;

class Keyboard : public AbstractCommand
{
	Q_OBJECT

private:
	
	// Vitesse du wifibot.
	int speed;
	// Widget permettant la capture des évènements du clavier.
	KeyboardCaptEvent *keyboardCaptEvent;

	// Position dernièrement enregistrées dans le fichier de configuration.
	QString moveForwardKey;
	QString moveBackwardKey;
	QString goRightKey;
	QString goLeftKey;
	QString speedUpKey;
	QString speedDownKey;

public:

	Keyboard(ConfigModel *configModel, Controler *controle, QObject *parent = 0);
	~Keyboard(void);

	void start(void);
	void stop(void);

	// On avance.
	void setSpeed(QKeyEvent *evt);

	void speedUp(void);
	void speedDown(void);
	void moveForward(void);
	void moveBackward(void);
	void goLeft(void);
	void goRight(void);
	void speedNulle(void);

public slots:

	// Toute modification du fichier de configuration entraine une mise à jour de la configuration de la commande.
	void onSavedConfig(void);
};

class KeyboardCaptEvent : public QWidget
{
private:

	Keyboard *keyboard;

public:

	KeyboardCaptEvent(Keyboard *keyboard, QWidget *parent = 0);

	// Appuie sur une des touches du clavier.
	void keyPressEvent(QKeyEvent *evt);
	// Relachement d'une des touches du clavier.
	void keyReleaseEvent(QKeyEvent *evt);
};

#endif
