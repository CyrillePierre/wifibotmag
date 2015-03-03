#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwt_compass.h>

#include "Thread.h"

class Window : public QMainWindow
{
    Q_OBJECT

private:
     QwtCompass *compass;
     QThread *thread;

public:
    Window();
    ~Window();
};

#endif // MAINWINDOW_H
