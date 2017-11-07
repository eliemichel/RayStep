#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>

QString loadCss(const QString & filename);

/*
* Roadmap :
* - Add uniforms
* - Add transforms
* - Add drag n drop primitives
*/

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyleSheet(loadCss("E:/SourceCode/Piced/share/styles/default.css"));

	MainWindow w;
	w.show();

	return a.exec();
}

QString loadCss(const QString & filename)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly);
	QTextStream in(&file);
	QString s = in.readAll();
	file.close();
	return s;
}