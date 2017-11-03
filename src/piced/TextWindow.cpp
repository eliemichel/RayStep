#include "TextWindow.h"
#include "ui_TextWindow.h"

#include "SceneGraph.h"
#include "SceneTreeModel.h"

#include <QFile>
#include <QMessageBox>
#include <QDebug>

TextWindow::TextWindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TextWindow)
{
	ui->setupUi(this);

	m_scene = new SceneTree;
	m_model = new SceneTreeModel(m_scene);
	ui->outliner->setModel(m_model);
	ui->outliner->expandAll();

	ui->properties->setModel(m_model);
	ui->properties->setSelectionModel(ui->outliner->selectionModel());

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
	return QString::fromStdString(m_scene->compileToGlsl("scene"));
	//return ui->shaderEdit->toPlainText();
}