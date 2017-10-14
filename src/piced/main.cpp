#include "MainWindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	qDebug() << "Hello World";

	return a.exec();
}
