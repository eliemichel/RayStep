#ifndef H_SCENETREEMODEL
#define H_SCENETREEMODEL

#include <QAbstractItemModel>

class SceneTree;

class SceneTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	enum DataRole {
		PropertyNameRole = Qt::UserRole, // Name of the property to which the current index column corresponds
		PropertyTypeRole, // Type of the property to which the current index column corresponds
		NbPropertiesRole // Number of properties in the given node
	};
	enum PropertyType {
		NoType,
		StringType,
		EnumType,
		TextType,
		IntType,
		FloatType
	};

public:
	explicit SceneTreeModel(SceneTree *scene, QObject *parent = Q_NULLPTR);
	~SceneTreeModel();

	// Read access
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Editable items
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	// Resizable models
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

	bool insertExistingRow(SceneTree *tree, int row, const QModelIndex &parent);

	// Drag and Drop
	QStringList mimeTypes() const override;
	QMimeData *mimeData(const QModelIndexList &indexes) const override;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
	Qt::DropActions supportedDropActions() const override;

	// Non-Qt-MVC accessors
	QString nameAt(const QModelIndex & index);

private:
	SceneTree *sceneTreeAt(const QModelIndex &index) const;

private:
	static bool isRoot(const QModelIndex &indext);
	constexpr static char* itemArrayMimeType = "application/x-raystep-scene-tree-item-array";

private:
	SceneTree *m_dataTree;
};

#endif // H_SCENETREEMODEL