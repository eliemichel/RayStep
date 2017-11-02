#include "SceneTreeModel.h"

#include "Logger.h"
#include "SceneGraph.h"

#include <QModelIndex>
#include <QDataStream>
#include <QMimeData>

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

	insertRows(0, 3, index(0, 0, QModelIndex()));
	
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

	if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count, destinationParent, destinationChild))
	{
		return false;
	}
	DEBUG_LOG << "Move " << count << " items from " << sourceTree->internalPath() << " at " << sourceRow << " toward " << destinationTree->internalPath() << " at " << destinationChild;
	for (size_t i = 0; i < count; ++i)
	{
		SceneTree * node = sourceTree->takeChild(sourceRow);
		destinationTree->addChild(node, destinationChild);
	}
	// Qt default drag manager calls a clearOrRemove() in abstract item view that removes the moved items, so add mock ones where it will remove them.
	for (size_t i = 0; i < count; ++i)
	{
		destinationTree->addChild(destinationChild);
	}
	endMoveRows();
	return true;
}

QStringList SceneTreeModel::mimeTypes() const
{
	return QStringList() << itemArrayMimeType;
}

QMimeData *SceneTreeModel::mimeData(const QModelIndexList &indexes) const
{
	if (indexes.empty())
	{
		return 0;
	}

	// A bit painful..

	QByteArray dataByteArray;
	QDataStream dataStream(&dataByteArray, QIODevice::WriteOnly);
	dataStream.writeRawData(reinterpret_cast<const char*>(&m_dataTree), sizeof(m_dataTree)); // First add root pointer to avoid copying around items between different instances of the tree
	dataStream << indexes.size();
	for (const QModelIndex & index : indexes)
	{
		dataStream << index.row() << index.column();
		void *p = index.internalPointer();
		dataStream.writeRawData(reinterpret_cast<const char*>(&p), sizeof(p));
	}

	QMimeData *mime = new QMimeData;
	mime->setData(itemArrayMimeType, dataByteArray);
	return mime;
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

		if (!data->hasFormat(itemArrayMimeType))
		{
			return false;
		}

		QByteArray dataByteArray = data->data(itemArrayMimeType);
		QDataStream dataStream(dataByteArray);
		SceneTree* root;
		dataStream.readRawData(reinterpret_cast<char*>(&root), sizeof(root));
		if (reinterpret_cast<SceneTree*>(root) != m_dataTree)
		{
			ERR_LOG << "Trying to move nodes between distinct SceneTree instances [" << (void*)root << " != " << (void*)m_dataTree << "]";
		}
		int size;
		dataStream >> size;
		if (size <= 0)
		{
			WARN_LOG << "Trying to move an empty list of nodes";
			return false; // Should not happen
		}


		// TODO: pack into ranges, here or in mimeData();
		for (int i = 0; i < size; ++i)
		{
			int r, c;
			void* p;
			dataStream >> r >> c;
			dataStream.readRawData(reinterpret_cast<char*>(&p), sizeof(p));
			QModelIndex source = createIndex(r, c, p);
			moveRows(source.parent(), source.row(), 1, parent, row);
		}

		return true;
	}
	return false;
	//return QAbstractItemModel::dropMimeData(data, action, row, column, parent);
}

Qt::DropActions SceneTreeModel::supportedDropActions() const
{
	return Qt::MoveAction;
}
