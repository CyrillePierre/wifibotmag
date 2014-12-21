/*
	configkeyboardpage.h
*/

#ifndef __CONFIGKEYBOARDPAGE_H__
#define __CONFIGKEYBOARDPAGE_H__

#include <QWidget>

class QLabel;
class QString;
class ConfigKeyboardLabel;

class ConfigKeyboardPage : public QWidget
{
	Q_OBJECT

public:
	
	ConfigKeyboardPage(QWidget *parent = 0);

	// Accesseurs pour le modèle de la configuration.
	QLabel *getMoveForwardCommand(void) const;
	QLabel *getMoveBackwardCommand(void) const;
	QLabel *getGoRightCommand(void) const;
	QLabel *getGoLeftCommand(void) const;
	QLabel *getSpeedUpCommand(void) const;
	QLabel *getSpeedDownCommand(void) const;

private:

	// Labels titre.
	QLabel *moveForwardLabel;
	QLabel *moveBackwardLabel;
	QLabel *goRightLabel;
	QLabel *goLeftLabel;
	QLabel *speedUpLabel;
	QLabel *speedDownLabel;

	// Labels qui contiennent les commandes du clavier.
	ConfigKeyboardLabel *moveForwardCommand;
	ConfigKeyboardLabel *moveBackwardCommand;
	ConfigKeyboardLabel *goRightCommand;
	ConfigKeyboardLabel *goLeftCommand;
	ConfigKeyboardLabel *speedUpCommand;
	ConfigKeyboardLabel *speedDownCommand;
};

#endif
