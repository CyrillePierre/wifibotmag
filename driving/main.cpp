#include "robot_wifibot.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Robot_Wifibot w;
	w.show();
	return a.exec();
}
