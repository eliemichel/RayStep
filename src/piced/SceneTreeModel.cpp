#include "SceneTreeModel.h"

#include "Logger.h"
#include "SceneGraph.h"

#include <QModelIndex>

SceneTreeModel::SceneTreeModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	// mock data
	m_dataTree = new SceneTree;
	m_dataTree->addChild();
	SceneTree *t = m_dataTree->child(0);
	t->setName("Union");
	t->addChild();
	t->addChild();
	t->addChild();
	t->child(0)->setName("sdSphere");
	t->child(1)->setName("sdBox");
	t->child(2)->setName("udRoundBox");
}

SceneTreeModel::~SceneTreeModel()
{

}

bool SceneTreeModel::isRoot(const QModelIndex &index)
{
	return !index.isValid();
}

SceneTree *SceneTreeModel::sceneTreeAt(const QModelIndex &index) const
{
	if (isRoot(index))
	{
		return m_dataTree;
	}
	//DEBUG_LOG << "Index(" << index.row() << ", " << index.column() << ", " << index.internalPointer() << ")";
	return static_cast<SceneTree*>(index.internalPointer());
}

QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	SceneTree *tree = sceneTreeAt(parent);
	if (!tree)
	{
		return QModelIndex();
	}

	if (row >= 0 && row < tree->childCount())
	{
		return createIndex(row, column, tree->child(row));
	}
	return QModelIndex();
}

QModelIndex SceneTreeModel::parent(const QModelIndex &index) const
{
	if (isRoot(index))
	{
		return QModelIndex();
	}

	SceneTree *tree = sceneTreeAt(index);
	if (!tree)
	{
		return QModelIndex();
	}
	if (tree->parent() == m_dataTree)
	{
		return QModelIndex();
	}
	return createIndex(0, 0, tree->parent());
}

int SceneTreeModel::rowCount(const QModelIndex &parent) const
{
	SceneTree *tree = sceneTreeAt(parent);
	if (!tree)
	{
		return 0;
	}
	return static_cast<int>(tree->childCount());
}

int SceneTreeModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant SceneTreeModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		SceneTree *tree = sceneTreeAt(index);
		if (!tree)
		{
			return QVariant();
		}
		DEBUG_LOG << "Index(" << index.row() << ", " << index.column() << ", " << index.internalPointer() << ")";
		return QString::fromStdString(tree->name());
	}
	return QVariant();
}
