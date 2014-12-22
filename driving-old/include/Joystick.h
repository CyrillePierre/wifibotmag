/*
    Joystick.h
*/

#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <QGraphicsItem>

#include "AbstractCommand.h"


class QObject;
class QGraphicsScene;
class QPainterPath;
class QPointF;
class QPainter;
class QTimer;
class QGraphicsView;
class QStyleOptionGraphicsItem;
class QGraphicsSceneMouseEvent;
class Controler;
class ConfigModel;
class Node;

class Joystick : public AbstractCommand
{
	Q_OBJECT

public:

	Joystick(ConfigModel *configModel, Controler *controler, QObject *parent = 0);
	~Joystick();

	void start(void);
	void stop(void);

private:
	
	// Fenêtre permettant d'afficher le Joystick virtuelle.
	QGraphicsView *joystickView;
	// Partie mobile du joystick.
	Node *node;

public slots:

	// Toute modification du fichier de configuration entraine une mise à jour de la configuration de la commande.
	void onSavedConfig(void);

};


class Node : public QGraphicsItem
{
public:

	Node(Controler *controler);
	
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	// Retourne la dermière position du noeud.
	inline QPointF& getLastPos(void) const;

protected:

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	
	// Controller pour commander le robot.
	Controler *controler;

};

class Center : public QGraphicsItem
{
public:
		
	Center(void);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

};

#endif
