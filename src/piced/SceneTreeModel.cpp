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
	SceneOperationNode *t = m_dataTree->addChild<SceneOperationNode>();
	t->setName("Union");
	t->setOperation(SceneOperationNode::UnionOp);

	ScenePrimitiveNode *prim;
	prim = t->addChild<ScenePrimitiveNode>();
	prim->setName("sdSphere");
	prim->setSource("vec2( sdSphere(    pos-vec3( 0.0,0.25, 0.0), 0.25 ), 46.9 )");
	prim = t->addChild<ScenePrimitiveNode>();
	prim->setName("sdBox");
	prim->setSource("vec2( sdBox(       pos-vec3( 1.0,0.25, 0.0), vec3(0.25) ), 3.0 )");
	prim = t->addChild<ScenePrimitiveNode>();
	prim->setName("udRoundBox");
	prim->setSource("vec2( udRoundBox(  pos-vec3( 1.0,0.25, 1.0), vec3(0.15), 0.1 ), 41.0 )");

	SceneOperationNode *dif = t->addChild<SceneOperationNode>();
	dif->setName("Difference");
	dif->setOperation(SceneOperationNode::DifferenceOp);
	
	prim = dif->addChild<ScenePrimitiveNode>();
	prim->setName("sdBox2");
	prim->setSource("vec2( sdBox(    pos-vec3( 0.0,0.25, 1.0), vec3(0.25) ), 15.0 )");

	prim = dif->addChild<ScenePrimitiveNode>();
	prim->setName("sdBigSphere");
	prim->setSource("vec2( sdSphere(       pos-vec3( 0.5,0.5, 0.5), 0.75 ), 3.0 )");


	
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
		//DEBUG_LOG << "Index(" << row << ", " << column << ", " << tree->child(row) << ")";
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
	if (role == Qt::DisplayRole || role == Qt::EditRole)
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

QVariant SceneTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && section == 0) {
		return QString("Outliner");
	}
	return QVariant();
}

Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags f = QAbstractItemModel::flags(index);
	if (!isRoot(index))
	{
		f = f
			| Qt::ItemIsSelectable
			| Qt::ItemIsEditable
			| Qt::ItemIsDragEnabled
			| Qt::ItemIsDropEnabled;
	}
	return f;
}

bool SceneTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role == Qt::EditRole)
	{
		SceneTree *tree = sceneTreeAt(index);
		if (!tree)
		{
			return false;
		}
		if (!value.canConvert<QString>())
		{
			return false;
		}
		// TODO avoid sibling with the same name
		tree->setName(value.toString().toStdString());
		emit dataChanged(index, index);
		return true;
	}
	return false;
}

bool SceneTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
	SceneTree *tree = sceneTreeAt(parent);
	if (!tree)
	{
		return false;
	}
	if (row < 0 || row > tree->childCount())
	{
		return false;
	}

	beginInsertRows(parent, row, row + count);
	DEBUG_LOG << "Insert " << count << " rows into " << tree->internalPath() << " at " << row;
	for (size_t i = 0; i < count; ++i)
	{
		SceneTree *child = tree->addChild(row);
		std::ostringstream ss;
		ss << "child #" << i;
		child->setName(ss.str());
	}
	endInsertRows();
	return true;
}

bool SceneTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	SceneTree *tree = sceneTreeAt(parent);
	if (!tree)
	{
		return false;
	}
	if (row < 0 || row + count >= tree->childCount())
	{
		return false;
	}

	beginRemoveRows(parent, row, row + count);
	DEBUG_LOG << "Remove " << count << " rows from " << tree->internalPath() << " at " << row;
	for (size_t i = 0; i < count; ++i)
	{
		tree->removeChild(row);
	}
	endRemoveRows();
	return true;
}

bool SceneTreeModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	SceneTree *sourceTree = sceneTreeAt(sourceParent);
	SceneTree *destinationTree = sceneTreeAt(destinationParent);
	if (!sourceTree || !destinationTree)
	{
		return false;
	}
	if (sourceRow < 0 || sourceRow + count >= sourceTree->childCount())
	{
		return false;
	}
	if (destinationChild < 0 || destinationChild >= destinationTree->childCount())
	{
		return false;
	}

	beginMoveRows(sourceParent, sourceRow, sourceRow + count, destinationParent, destinationChild);
	sourceTree->child(sourceRow);
	for (size_t i = 0; i < count; ++i)
	{
		DEBUG_LOG << "Move " << count << " items from " << sourceTree->internalPath() << " at " << sourceRow << " toward " << destinationTree->internalPath() << " at " << destinationChild;
		SceneTree * node = sourceTree->takeChild(sourceRow);
		destinationTree->addChild(node, destinationChild);
	}
	endMoveRows();
	return true;
}

bool SceneTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	if (action == Qt::MoveAction)
	{
		SceneTree *tree = sceneTreeAt(parent);
		if (!tree)
		{
			return false;
		}

		//tree->addChild

		//return true;
	}
	return QAbstractItemModel::dropMimeData(data, action, row, column, parent);
}

Qt::DropActions SceneTreeModel::supportedDropActions() const
{
	return Qt::MoveAction;
}
