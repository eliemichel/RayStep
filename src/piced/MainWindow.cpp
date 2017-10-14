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

void MainWindow::wheelEvent(QWheelEvent * event)
{
	//m_scale *= 1.1f;
	updateImageSize();
}

void MainWindow::showOpenFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
	ui->image->setPixmap(QPixmap(fileName));
	fitImageSize();
}

void MainWindow::fitImageSize()
{
	if (!ui->image->pixmap())
	{
		return;
	}

	QSize imSize = ui->image->pixmap()->size();
	QSize winSize = ui->scrollArea->viewport()->size();
	m_scale = qMin(static_cast<float>(winSize.width()) / imSize.width(), static_cast<float>(winSize.height()) / imSize.height());
	updateImageSize();
}

void MainWindow::updateImageSize()
{
	if (!ui->image->pixmap())
	{
		return;
	}

	qDebug() << m_scale;
	ui->image->resize(m_scale * ui->image->pixmap()->size());
}

