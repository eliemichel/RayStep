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
	connect(m_sourceModel, &QAbstractItemModel::dataChanged, this, &NodePropertiesModel::onDataChanged);
}

void NodePropertiesModel::setSourceSelectionModel(QItemSelectionModel *selectionModel)
{
	m_sourceSelectionModel = selectionModel;
	connect(m_sourceSelectionModel, &QItemSelectionModel::currentChanged, this, &NodePropertiesModel::onCurrentChanged);
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
	if (role != Qt::DisplayRole && role != Qt::EditRole)
	{
		return QVariant();
	}

	int sourceColumn = index.row() + 1; // 0 is reserved
	const QModelIndex & current = sourceSelectionModel()->currentIndex();
	const QModelIndex & currentColumn = sourceModel()->index(current.row(), sourceColumn, current.parent());
	switch (index.column())
	{
	case LabelColumn:
		return sourceModel()->data(currentColumn, SceneTreeModel::PropertyNameRole);
	case ValueColumn:
		return sourceModel()->data(currentColumn);
	default:
		return QVariant();
	}
}

QVariant NodePropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
	{
		return QVariant();
	}

	switch (section)
	{
	case LabelColumn:
		return "Property";
	case ValueColumn:
		return "Values";
	default:
		return QVariant();
	}
}

Qt::ItemFlags NodePropertiesModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags f = QAbstractItemModel::flags(index);
	if (index.isValid() && index.column() == ValueColumn)
	{
		f |= Qt::ItemIsEditable;
	}
	return f;
}

bool NodePropertiesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || index.column() != ValueColumn)
	{
		return false;
	}

	int sourceColumn = index.row() + 1; // 0 is reserved
	const QModelIndex & current = sourceSelectionModel()->currentIndex();
	const QModelIndex & currentColumn = sourceModel()->index(current.row(), sourceColumn, current.parent());
	sourceModel()->setData(currentColumn, value);
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
