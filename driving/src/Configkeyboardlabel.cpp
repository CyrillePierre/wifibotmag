/*
	configkeyboardlabel.cpp
*/

#include "Configkeyboardlabel.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QFont>
#include <QPalette>
#include <QBrush>
#include <QColor>

ConfigKeyboardLabel::ConfigKeyboardLabel(QWidget *parent)
    : QLabel(parent)
{
	// On ne prend pas en compte les évènements du clavier.
	this->releaseKeyboard();

	// Initialisation des caractéristiques du texte écrit.
	QFont font;
	font.setBold(true);
	this->setFont(font);
	
	QPalette palette;
	palette.setBrush(QPalette::WindowText, QBrush(QColor(88, 121, 183)));
	palette.setBrush(QPalette::Active, QPalette::Window, QBrush(QColor(88, 121, 183)));
	this->setPalette(palette);	
}

void ConfigKeyboardLabel::mousePressEvent(QMouseEvent * event)
{
	// Vérification du boutton qui a été appuyé.
	if (event->button() == Qt::LeftButton)
	{
		// Effaçage de la valeur courante.
		this->clear();
		// On informe l'utilisateur qu'il doit appuyer sur une touche.
		this->setText(tr("Appuyez sur une touche"));
		// L'appuie d'une touche est pris en compte.
		this->grabKeyboard();
	}
}

void ConfigKeyboardLabel::keyPressEvent(QKeyEvent * event)
{
	// On initialise le texte dans le label.
	this->setText(event->text());
	// On ne prend plus en compte les évènements du clavier.
	this->releaseKeyboard();
}