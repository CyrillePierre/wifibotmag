/*
	configkeyboardpage.cpp
*/

#include <QtGui>

#include "Configkeyboardpage.h"
#include "Configkeyboardlabel.h"

ConfigKeyboardPage::ConfigKeyboardPage(QWidget *parent)
    : QWidget(parent)
{
	QLabel *titleLabel = new QLabel;
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setWeight(75);
	titleLabel->setFont(font);
	titleLabel->setText(tr("Configuration du Clavier"));

	moveForwardLabel = new QLabel(tr("Avancer"), this);
	moveBackwardLabel = new QLabel(tr("Reculer"), this);
	goRightLabel = new QLabel(tr("Droite"), this);
	goLeftLabel = new QLabel(tr("Gauche"), this);

	moveForwardCommand = new ConfigKeyboardLabel(this);
	moveBackwardCommand = new ConfigKeyboardLabel(this);
	goRightCommand = new ConfigKeyboardLabel(this);
	goLeftCommand = new ConfigKeyboardLabel(this);
	
	QGridLayout *labelLayout = new QGridLayout;
	labelLayout->addWidget(moveForwardLabel, 0, 0);
	labelLayout->addWidget(moveBackwardLabel, 1, 0);
	labelLayout->addWidget(goRightLabel, 2, 0);
	labelLayout->addWidget(goLeftLabel, 3, 0);
	labelLayout->addWidget(moveForwardCommand, 0, 1);
	labelLayout->addWidget(moveBackwardCommand, 1, 1);
	labelLayout->addWidget(goRightCommand, 2, 1);
	labelLayout->addWidget(goLeftCommand, 3, 1);
	
	QGroupBox *configCommandGroup = new QGroupBox(tr("Mouvements"));
	configCommandGroup->setLayout(labelLayout);

	speedUpLabel = new QLabel(tr("Passer une vitesse"), this);
	speedDownLabel = new QLabel(tr("Rétrograder"), this);

	speedUpCommand = new ConfigKeyboardLabel(this);
	speedDownCommand = new ConfigKeyboardLabel(this);

	QGridLayout *labelSpeedLayout = new QGridLayout;
	labelSpeedLayout->addWidget(speedUpLabel, 0, 0);
	labelSpeedLayout->addWidget(speedDownLabel, 1, 0);
	labelSpeedLayout->addWidget(speedUpCommand, 0, 1);
	labelSpeedLayout->addWidget(speedDownCommand, 1, 1);

	QGroupBox *configSpeedGroup = new QGroupBox(tr("Vitesse"));
	configSpeedGroup->setLayout(labelSpeedLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(titleLabel);
	mainLayout->addSpacing(10);
	mainLayout->addWidget(configCommandGroup);
	mainLayout->addSpacing(6);
	mainLayout->addWidget(configSpeedGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
}

QLabel *ConfigKeyboardPage::getMoveForwardCommand() const
{
	return moveForwardCommand;
}

QLabel *ConfigKeyboardPage::getMoveBackwardCommand() const
{
	return moveBackwardCommand;
}

QLabel *ConfigKeyboardPage::getGoRightCommand() const
{
	return goRightCommand;
}

QLabel *ConfigKeyboardPage::getGoLeftCommand() const
{
	return goLeftCommand;
}

QLabel *ConfigKeyboardPage::getSpeedUpCommand() const
{
	return speedUpCommand;
}

QLabel *ConfigKeyboardPage::getSpeedDownCommand() const
{
	return speedDownCommand;
}

