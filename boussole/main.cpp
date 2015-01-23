#include "window.h"
#include <QApplication>

#include <qwt_plot.h>
#include <qwt_compass.h>
#include <qwt_compass_rose.h>
#include <qwt_dial_needle.h>
#include <QSpinBox>
#include <QThread>
#include <QHBoxLayout>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window window;

    window.setFixedSize(500, 500);
    window.show();

    return a.exec();
}
