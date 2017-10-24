#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(showOpenFileDialog()));

	connect(ui->text, SIGNAL(submitted()), this, SLOT(updateShader()));

	ui->splitter->setSizes(QList<int>() << 200 << 100);

	// demo
	ui->text->load("E:/SourceCode/Piced/share/shaders/include/default-scene.inc.glsl");
}

MainWindow::~MainWindow()
{
}

void MainWindow::showOpenFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Shader"), "", tr("Shader Files (*.txt *.glsl)"));
	ui->text->load(fileName);
}


void MainWindow::updateShader()
{
	qDebug() << "Update shader: " << ui->text->content();
}
