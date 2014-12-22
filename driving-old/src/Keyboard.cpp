/*
    Keyboard.cpp
*/

#include "Controler.h"
#include "Configmodel.h"
#include "Keyboard.h"

#include <QKeyEvent>

Keyboard::Keyboard(ConfigModel *configModel, Controler *controle, QObject *parent)
	: AbstractCommand(configModel, controle, parent)
{
	// Initialisation de la vitesse.
	speed = 1;

	// Pour pouvoir mettre à jour les élément dépendant de la configuration.
	connect(configModel, SIGNAL(update()), this, SLOT(onSavedConfig()));
}

Keyboard::~Keyboard()
{}

void Keyboard::start(void)
{
	// Création de la widget qui capte les évènements.
	keyboardCaptEvent = new KeyboardCaptEvent(this);
}

void Keyboard::stop(void)
{
	// Destruction de la widget qui capte les évènements.
	if (keyboardCaptEvent)
		delete keyboardCaptEvent;
}


void Keyboard::setSpeed(QKeyEvent *evt)
{
	if (evt->text() == moveForwardKey)
		this->moveForward();
	if (evt->text() == moveBackwardKey)
		this->moveBackward();
	if (evt->text() == goRightKey)
		this->goRight();
	if (evt->text() == goLeftKey)
		this->goLeft();
	if (evt->text() == speedUpKey)
		this->speedUp();
	if (evt->text() == speedDownKey)
		this->speedDown();
}

void Keyboard::moveForward()
{
	controler->setSpeedLeft(20*speed);
	controler->setSpeedRight(20*speed);
}

void Keyboard::moveBackward()
{
	controler->setSpeedLeft(-20*speed);
	controler->setSpeedRight(-20*speed);
}

void Keyboard::goLeft()
{
	controler->setSpeedLeft(-20*speed);
	controler->setSpeedRight(20*speed);
}

void Keyboard::goRight()
{
    controler->setSpeedLeft(20*speed);
    controler->setSpeedRight(-20*speed);
}

void Keyboard::speedNulle()
{
  controler->setSpeedLeft(0);
  controler->setSpeedRight(0);
}

void Keyboard::speedUp()
{
	if (speed < 3)
		speed ++;
}

void Keyboard::speedDown()
{
	if (speed > 0)
		speed --;
}

void Keyboard::onSavedConfig()
{
	// Récupération de la nouvelle configuration du clavier.
	moveForwardKey = configModel->getMoveForwardKey();
	moveBackwardKey = configModel->getMoveBackwardKey();
	goRightKey = configModel->getGoRightKey();
	goLeftKey = configModel->getGoLeftKey();
	speedUpKey = configModel->getSpeedUpKey();
	speedDownKey = configModel->getSpeedDownKey();
}

// METHODES DE KEYBOARDCAPTEVENT

KeyboardCaptEvent::KeyboardCaptEvent(Keyboard *keyboard, QWidget *parent)
	: QWidget(parent)
{
	this->keyboard = keyboard;
	this->grabKeyboard();
}

void KeyboardCaptEvent::keyPressEvent(QKeyEvent *evt)
{
	if (keyboard)
		keyboard->setSpeed(evt);
}

void KeyboardCaptEvent::keyReleaseEvent(QKeyEvent *evt)
{
	if (keyboard)
		keyboard->speedNulle();
}
