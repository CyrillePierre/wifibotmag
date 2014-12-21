/*
	configkeyboardlabel.h
*/

#ifndef __CONFIGKEYBOARDLABEL_H__
#define __CONFIGKEYBOARDLABEL_H__

#include <QLabel>

class QMouseEvent;
class QKeyEvent;
class QWidget;

class ConfigKeyboardLabel : public QLabel
{
	Q_OBJECT

public:

	ConfigKeyboardLabel(QWidget *parent = 0);

protected:

	// Lorsque l'on clique sur la widget.
	void mousePressEvent(QMouseEvent * event);
	// Lorsque l'on appuie sur une touche.
	void keyPressEvent(QKeyEvent * event);
};

#endif
