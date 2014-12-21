#include "window.h"

#include <qwt_plot.h>
#include <qwt_compass.h>
#include <qwt_compass_rose.h>
#include <qwt_dial_needle.h>
#include <QSpinBox>
#include <QThread>
#include <QHBoxLayout>

#include "Thread.h"

Window::Window() : QMainWindow()
{
    compass = new QwtCompass(this);
    setCentralWidget(compass);

    QMap<double, QString> map;

    for ( double d = 0; d < 360; d += 30.0 )
    {
        QString label;
        label.sprintf( "%.0f", d );
        map.insert( d, label );
    }

    QwtCompassScaleDraw *scaleDraw = new QwtCompassScaleDraw( map );

    scaleDraw->enableComponent(QwtAbstractScaleDraw::Ticks, true);
    scaleDraw->enableComponent(QwtAbstractScaleDraw::Labels, true);
    scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, true);
    scaleDraw->setTickLength(QwtScaleDiv::MinorTick, 0);
    scaleDraw->setTickLength(QwtScaleDiv::MediumTick, 0);
    scaleDraw->setTickLength(QwtScaleDiv::MajorTick, 3);

    compass->setScaleDraw(scaleDraw);
    compass->setScaleMaxMinor(5);
    compass->setLineWidth(2);
    compass->setValue(20);

    QwtSimpleCompassRose *rose = new QwtSimpleCompassRose(8, 1);
    compass->setRose(rose);
    compass->setNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow,
                                                 true, Qt::red));
    thread = new Thread();
    thread->start();

    QObject::connect(this, SIGNAL(destroyed()), thread, SLOT(terminate()));
    QObject::connect(thread, SIGNAL(changed2(double)), compass, SLOT(setValue(double)));
}

Window::~Window()
{
    delete thread;
    delete compass;
}
