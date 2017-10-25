#include "SceneTreeModel.h"

#include "Logger.h"
#include "SceneGraph.h"

#include <QModelIndex>

SceneTreeModel::SceneTreeModel(SceneTree *scene, QObject *parent)
	: QAbstractItemModel(parent)
	, m_dataTree(scene)
{
	// mock data
	m_dataTree = scene;
	m_dataTree->addChild<SceneOperationNode>();
	SceneOperationNode *t = static_cast<SceneOperationNode*>(m_dataTree->child(0));
	t->setName("Union");
	t->setOperation(SceneOperationNode::UnionOp);
	t->addChild<ScenePrimitiveNode>();
	t->addChild<ScenePrimitiveNode>();
	t->addChild<ScenePrimitiveNode>();
	t->child(0)->setName("sdSphere");
	static_cast<ScenePrimitiveNode*>(t->child(0))->setSource("vec2( sdSphere(    pos-vec3( 0.0,0.25, 0.0), 0.25 ), 46.9 )");
	t->child(1)->setName("sdBox");
	static_cast<ScenePrimitiveNode*>(t->child(1))->setSource("vec2( sdBox(       pos-vec3( 1.0,0.25, 0.0), vec3(0.25) ), 3.0 )");
	t->child(2)->setName("udRoundBox");
	static_cast<ScenePrimitiveNode*>(t->child(2))->setSource("vec2( udRoundBox(  pos-vec3( 1.0,0.25, 1.0), vec3(0.15), 0.1 ), 41.0 )");
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
		return QString::fromStdString(tree->name());
	}
	return QVariant();
}
