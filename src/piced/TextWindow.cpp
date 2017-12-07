#include "TextWindow.h"
#include "ui_TextWindow.h"

#include "SceneGraph.h"
#include "SceneTreeModel.h"
#include "NodePropertiesModel.h"
#include "UniformsModel.h"

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

	m_propertiesModel = new NodePropertiesModel();
	m_propertiesModel->setSourceModel(m_model);
	m_propertiesModel->setSourceSelectionModel(ui->outliner->selectionModel());
	ui->propertiesView->setModel(m_propertiesModel);
	ui->propertiesView->expandAll();

	m_uniformsModel = new UniformsModel();
	ui->uniformsView->setModel(m_uniformsModel);

	connect(ui->submitButton, &QPushButton::clicked, this, &TextWindow::submitted);
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

	QString content = in.readAll();
	Q_UNUSED(content)
		// TODO: make an actual file format and loading

	file.close();
}

QString TextWindow::content() const
{
	return QString::fromStdString(m_scene->compileToGlsl("scene"));
}

QString TextWindow::uniforms() const
{
	return QString::fromStdString(m_uniformsModel->compileToGlsl());
}
