#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "augen/Actorio.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->actionQuit, &QAction::triggered, this, &QWidget::close);
	connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::showOpenFileDialog);

	connect(ui->text, &TextWindow::submitted, this, &MainWindow::updateShader);

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
	Actorio *actorio = ui->viewport->actorio();
	if (actorio) {
		actorio->updateShader(ui->text->content().toStdString());
	}
}
