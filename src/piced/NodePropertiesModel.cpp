#include "NodePropertiesModel.h"
#include "SceneTreeModel.h"
#include "Logger.h"

#include <QItemSelectionModel>

NodePropertiesModel::NodePropertiesModel(QObject *parent)
	: QAbstractItemModel(parent)
{
}

NodePropertiesModel::~NodePropertiesModel()
{
}

void NodePropertiesModel::setSourceModel(SceneTreeModel *model)
{
	m_sourceModel = model;
	connect(m_sourceModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)),
		this, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)));
}

void NodePropertiesModel::setSourceSelectionModel(QItemSelectionModel *selectionModel)
{
	m_sourceSelectionModel = selectionModel;
	connect(m_sourceSelectionModel, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
		this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
}


QModelIndex NodePropertiesModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		return QModelIndex(); // No children in this model
	}
	else
	{
		return createIndex(row, column);
	}
}

QModelIndex NodePropertiesModel::parent(const QModelIndex &index) const
{
	// No children in this model
	Q_UNUSED(index);
	return QModelIndex();
}

int NodePropertiesModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		return 0; // No children in this model
	}
	else
	{
		const QModelIndex & current = sourceSelectionModel()->currentIndex();
		return sourceModel()->data(current, SceneTreeModel::NbPropertiesRole).toInt();

	}
}

int NodePropertiesModel::columnCount(const QModelIndex &parent) const
{
	return 2; // Label and input
}

QVariant NodePropertiesModel::data(const QModelIndex &index, int role) const
{
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	int sourceColumn = index.row() + 1; // 0 is reserved
	const QModelIndex & current = sourceSelectionModel()->currentIndex();
	const QModelIndex & currentColumn = sourceModel()->index(current.row(), sourceColumn, current.parent());
	switch (index.column())
	{
	case 0: // labels
		return sourceModel()->data(currentColumn, SceneTreeModel::PropertyNameRole);
	case 1: // values
		return sourceModel()->data(currentColumn);
	default:
		return QVariant();
	}
}


void NodePropertiesModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
	// This is a quite general tree update check, maybe it should be abstracted and factorized
	// (duplicate in NodeProperties)

	const QModelIndex & current = sourceSelectionModel()->currentIndex();
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

void NodePropertiesModel::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(current);
	Q_UNUSED(previous);
	updateContent();
}

void NodePropertiesModel::updateContent(const QVector<int> &roles)
{
	beginResetModel();
	endResetModel();
}
