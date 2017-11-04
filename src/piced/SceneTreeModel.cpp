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
}

SceneTreeModel::~SceneTreeModel()
{

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
	SceneTree *tree = sceneTreeAt(index);
	if (!tree)
	{
		return QVariant();
	}

	switch (role)
	{
	case Qt::DisplayRole:
	case Qt::EditRole:
		return QString::fromStdString(tree->name());
	case PropertyNameRole:
		switch (index.column())
		{
		case 0:
			return "<system>";
		case 1:
			return "Name";
		case 2:
			return "Type";
		default:
			return QVariant();
		}
	case PropertyTypeRole:
		switch (index.column())
		{
		case 0:
			return NoType;
		case 1:
			return StringType;
		case 2:
			return EnumType;
		default:
			return QVariant();
		}
	default:
		return QVariant();
	}
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

	beginInsertRows(parent, row, row + count - 1);
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

	beginRemoveRows(parent, row, row + count - 1);
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
		ERR_LOG << "Invalid source or destination item";
		return false;
	}
	if (sourceRow < 0 || sourceRow + count - 1 >= sourceTree->childCount())
	{
		ERR_LOG << "Invalid source row [" << sourceRow << "] and/or count [" << count << "] for a child count of " << sourceTree->childCount();
		return false;
	}
	if (destinationChild < 0 || destinationChild > destinationTree->childCount())
	{
		ERR_LOG << "Invalid destination row: " << destinationChild;
		return false;
	}

	DEBUG_LOG << "Move " << count << " items from " << sourceTree->internalPath() << " at " << sourceRow << " toward " << destinationTree->internalPath() << " at " << destinationChild;
	if (!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild))
	{
		ERR_LOG << "Invalid move";
		return false;
	}
	for (size_t i = 0; i < count; ++i)
	{
		SceneTree * node = sourceTree->takeChild(sourceRow);
		destinationTree->addChild(node, destinationChild);
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
	if (action == Qt::CopyAction)
	{
		DEBUG_LOG << "CopyAction";
		SceneTree *tree = sceneTreeAt(parent);
		if (!tree)
		{
			ERR_LOG << "Invalid drop parent";
			return false;
		}

		if (row == -1)
		{
			row = static_cast<int>(tree->childCount());
		}

		if (!data->hasFormat(itemArrayMimeType))
		{
			ERR_LOG << "Invalid drop data: no field with format " << itemArrayMimeType;
			return false;
		}

		QByteArray dataByteArray = data->data(itemArrayMimeType);
		QDataStream dataStream(dataByteArray);
		SceneTree* root;
		dataStream.readRawData(reinterpret_cast<char*>(&root), sizeof(root));
		if (reinterpret_cast<SceneTree*>(root) != m_dataTree)
		{
			ERR_LOG << "Trying to move nodes between distinct SceneTree instances [" << (void*)root << " != " << (void*)m_dataTree << "]";
			return false;
		}
		int size;
		dataStream >> size;
		if (size <= 0)
		{
			ERR_LOG << "Trying to move an empty list of nodes";
			return false; // Should not happen
		}

		if (tree->childCount() + size > tree->maxChildren())
		{
			WARN_LOG << "Cannot drop more items than " << tree->maxChildren();
			return false;
		}

		DEBUG_LOG << "Dropping " << size << " nodes";

		// TODO: pack into ranges, here or in mimeData();
		for (int i = 0; i < size; ++i)
		{
			int r, c;
			void* p;
			dataStream >> r >> c;
			dataStream.readRawData(reinterpret_cast<char*>(&p), sizeof(p));
			QModelIndex source = createIndex(r, c, p);
			if (!moveRows(source.parent(), source.row(), 1, parent, row))
			{
				return false;
			}
		}

		return true;
	}
	return false;
	//return QAbstractItemModel::dropMimeData(data, action, row, column, parent);
}

Qt::DropActions SceneTreeModel::supportedDropActions() const
{
	return Qt::CopyAction;
}

QString SceneTreeModel::nameAt(const QModelIndex & index)
{
	SceneTree *tree = sceneTreeAt(index);
	if (!tree)
	{
		return QString();
	}

	return QString::fromStdString(tree->name());
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
