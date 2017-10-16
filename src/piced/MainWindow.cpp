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
}

MainWindow::~MainWindow()
{
}

void MainWindow::showOpenFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
}
