#include <QApplication>
#include <QMainWindow>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QMainWindow w;
	w.show();

	qDebug() << "Hello World";

	return a.exec();
}
