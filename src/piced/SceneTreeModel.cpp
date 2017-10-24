#include "SceneTreeModel.h"

#include "Logger.h"

#include <QModelIndex>

SceneTreeModel::SceneTreeModel(QObject *parent)
	: QAbstractItemModel(parent)
{

}

SceneTreeModel::~SceneTreeModel()
{

}

bool SceneTreeModel::isRoot(const QModelIndex &index)
{
	return !index.isValid();
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (isRoot(parent) && row < 3)
	{
		return createIndex(row, column, nullptr);
	}
	return QModelIndex();
}

QModelIndex SceneTreeModel::parent(const QModelIndex &index) const
{
	return QModelIndex();
}

int SceneTreeModel::rowCount(const QModelIndex &parent) const
{
	DEBUG_LOG << "isRoot valid?" << parent.isValid() << ", " << parent.row() << ", " << parent.column() << ", " << parent.internalPointer();
	if (isRoot(parent))
	{
		return 3;
	}
	return 0;
}

int SceneTreeModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant SceneTreeModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		return QString("yeah");
	}
	return QVariant();
}
