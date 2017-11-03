#include "NodeProperties.h"
#include "ui_NodeProperties.h"
#include "SceneTreeModel.h"

#include <QItemSelectionModel>

NodeProperties::NodeProperties(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::NodeProperties)
{
	ui->setupUi(this);

	connect(ui->nameInput, SIGNAL(textChanged(const QString&)), this, SLOT(onNameChanged(const QString &)));
}

NodeProperties::~NodeProperties()
{
}


void NodeProperties::setModel(SceneTreeModel *model)
{
	m_model = model;
	connect(m_model, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)),
		this, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)));
}

void NodeProperties::setSelectionModel(QItemSelectionModel *selectionModel)
{
	m_selectionModel = selectionModel;
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
}

void NodeProperties::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
	// This is a quite general tree update check, maybe it should be abstracted and factorized

	const QModelIndex & current = selectionModel()->currentIndex();
	QModelIndex x = current;

	// Is current index child of changed range
	while (x.isValid() && x.parent() != topLeft.parent())
	{
		x = x.parent();
	}
	if (x.isValid()) {
		if (x.row() >= topLeft.row() && x.row() <= bottomRight.row())
		{
			updateContent(roles);
		}
		return;
	}

	// Is current index parent of the changed range
	x = topLeft;
	while (x.isValid() && x.parent() != current)
	{
		x = x.parent();
	}
	if (x.isValid()) {
		updateContent(roles);
	}
}

void NodeProperties::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(current);
	Q_UNUSED(previous);
	updateContent();
}

void NodeProperties::updateContent(const QVector<int> &roles)
{
	QString name = model()->nameAt(selectionModel()->currentIndex());
	ui->nameInput->setText(name);
}

void NodeProperties::onNameChanged(const QString &name)
{
	model()->setData(selectionModel()->currentIndex(), name, Qt::EditRole);
}
