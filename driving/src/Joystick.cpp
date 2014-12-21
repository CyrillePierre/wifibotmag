/*
    Joystick.cpp
*/

#include "Joystick.h"
#include "Controler.h"
#include "Configmodel.h"

#include <QSize>
#include <QGraphicsScene>
#include <QPainterPath>
#include <QPointF>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QTimer>

Joystick::Joystick(ConfigModel *configModel, Controler *controler, QObject *parent)
    : AbstractCommand(configModel, controler, parent)
{	

	// Créaton de la vue du Joystick.
	joystickView = new QGraphicsView;

	// Construction de la scène.
	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	scene->setSceneRect(-75, -100, 150, 200);
	
	joystickView->setScene(scene);
	joystickView->setCacheMode(QGraphicsView::CacheBackground);
	joystickView->setRenderHint(QPainter::Antialiasing);
	joystickView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	joystickView->setResizeAnchor(QGraphicsView::AnchorViewCenter);

	// Création du joystick.
	node = new Node(controler);
	node->setPos(0,0);
	scene->addItem(node);
	Center *center = new Center;
	center->setPos(0,0);
	scene->addItem(center);

	joystickView->setFixedSize(QSize(160,210));
	joystickView->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::WindowTitleHint);
	joystickView->setWindowTitle(tr("Joystick"));

	// Pour pouvoir mettre à jour les élément dépendant de la configuration.
	connect(configModel, SIGNAL(update()), this, SLOT(onSavedConfig()));
}

Joystick::~Joystick()
{
	// Destruction de la vue.
	if (joystickView)
		delete joystickView;
	if (node)
		delete node;
}

void Joystick::start()
{
	// Affichage du joystick.
	joystickView->show();
}

void Joystick::stop()
{
	// On cache le joystick.
	joystickView->hide();
}

void Joystick::onSavedConfig()
{
}

// METHODES DE NODE.

Node::Node(Controler *controler)
{
	setFlag(ItemIsMovable);
	setZValue(1);

	this->controler = controler;
}

QPainterPath Node::shape() const
{
	QPainterPath path;
	path.addEllipse(-10, -10, 20, 20);
	return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::darkGray);
	painter->save();
	painter->setOpacity(0.5);
	painter->drawEllipse(-7, -7, 20, 20);
	painter->restore();

	QRadialGradient gradient(-3, -3, 10);
	if (option->state & QStyle::State_Sunken) {
		gradient.setCenter(3, 3);
		gradient.setFocalPoint(3, 3);
		gradient.setColorAt(1, QColor(Qt::gray).light(120));
		gradient.setColorAt(0, QColor(Qt::black).light(120));
	} else {
		gradient.setColorAt(0, Qt::gray);
		gradient.setColorAt(1, Qt::black);	
	}
	painter->setBrush(gradient);
	painter->setPen(QPen(Qt::black, 0));
	painter->drawEllipse(-10, -10, 20, 20);
}

QRectF Node::boundingRect() const
{
	qreal adjust = 2;
	return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	int rightSpeed;
	int leftSpeed;

	update();

	// On fait bouger le robot.
	rightSpeed = (int)(-event->scenePos().y());
	leftSpeed = (int)(-event->scenePos().y());

	// On tourne.
	rightSpeed -= (int)(event->scenePos().x()*0.75);
	leftSpeed += (int)(event->scenePos().x()*0.75);
		
	// On envoie les ordres au controleur.
	controler->setSpeedLeft(leftSpeed);
	controler->setSpeedRight(rightSpeed);

	QGraphicsItem::mouseMoveEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	controler->setSpeedLeft(0.0);
	controler->setSpeedRight(0.0);
	QGraphicsItem::mouseReleaseEvent(event);
	setPos(0,0);
} 


// METHODES DE CENTER

Center::Center()
{}

void Center::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	painter->setPen(Qt::NoPen);

	QRadialGradient gradient(0, 0, 40);
	gradient.setColorAt(0, Qt::lightGray);
	gradient.setColorAt(1, Qt::darkGreen);

	painter->setBrush(gradient);
	painter->drawLine(-10, -20, 20, 30);

	QPainterPath pathRight;
	pathRight.moveTo(3, 0);
	pathRight.lineTo(25, -3);
	pathRight.lineTo(25, 3);
	pathRight.lineTo(3, 0);
	painter->drawPath(pathRight);

	QPainterPath pathLeft;
	pathLeft.moveTo(-3, 0);
	pathLeft.lineTo(-25, -3);
	pathLeft.lineTo(-25, 3);
	pathLeft.lineTo(-3, 0);
	painter->drawPath(pathLeft);

	QPainterPath pathTop;
	pathTop.moveTo(0, -3);
	pathTop.lineTo(-3, -25);
	pathTop.lineTo(3, -25);
	pathTop.lineTo(0, -3);
	painter->drawPath(pathTop);

	QPainterPath pathBottom;
	pathBottom.moveTo(0, 3);
	pathBottom.lineTo(3, 25);
	pathBottom.lineTo(-3, 25);
	pathBottom.lineTo(0, 3);
	painter->drawPath(pathBottom);
	
	painter->setBrush(Qt::NoBrush);
	QPen pen(Qt::darkGreen);
	pen.setStyle(Qt::DashDotLine);
	painter->setPen(pen);	
	painter->drawEllipse(QRect(-15, -15, 30, 30));
}

QRectF Center::boundingRect() const
{
	return QRectF(-50, -50, 100, 100);
}

QPainterPath Center::shape() const
{
	QPainterPath path;
	path.addEllipse(-50, -50, 100, 100);
	return path;
}
