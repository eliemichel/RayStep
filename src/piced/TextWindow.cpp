#include "TextWindow.h"
#include "ui_TextWindow.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>

TextWindow::TextWindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TextWindow)
{
	ui->setupUi(this);

	connect(ui->submitButton, SIGNAL(clicked()), this, SIGNAL(submitted()));
}

TextWindow::~TextWindow()
{
}

void TextWindow::load(const QString & filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(0, "error", file.errorString());
	}

	QTextStream in(&file);

	ui->shaderEdit->setPlainText(in.readAll());

	file.close();
}

QString TextWindow::content() const
{
	return ui->shaderEdit->toPlainText();
}