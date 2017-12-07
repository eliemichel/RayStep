#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "augen/Actorio.h"
#include "Logger.h"
#include "UniformsModel.h"

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
	connect(ui->text->uniformsModel(), &UniformsModel::valueChanged, this, &MainWindow::updateUniform);

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
	Actorio *actorio = ui->viewport->actorio();
	if (actorio) {
		std::string content = ui->text->content().toStdString();
		std::string uniforms = ui->text->uniforms().toStdString();
		DEBUG_LOG << content;
		actorio->updateShader(content, uniforms);
	}
}


void MainWindow::updateUniform(std::string name, float value)
{
	Actorio *actorio = ui->viewport->actorio();
	if (actorio) {
		actorio->setUniform(name, value);
	}
}

