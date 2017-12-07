#ifndef H_NODEPROPERTIESMODEL
#define H_NODEPROPERTIESMODEL

#include <QAbstractItemModel>

class SceneTreeModel;
class QItemSelectionModel;

/**
 * This model proxies a given element (or maybe one day selection) of a SceneTreeModel to present its properties as lines.
 * It is intended to use with a QListView-like view.
 */
class NodePropertiesModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	enum Column {
		LabelColumn,
		ValueColumn
	};

public:
	explicit NodePropertiesModel(QObject *parent = Q_NULLPTR);
	~NodePropertiesModel();

	inline SceneTreeModel *sourceModel() const { return m_sourceModel; }
	void setSourceModel(SceneTreeModel *model);

	inline QItemSelectionModel *sourceSelectionModel() const { return m_sourceSelectionModel; }
	void setSourceSelectionModel(QItemSelectionModel *selectionModel);

public:
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

private slots:
	void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
	void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
	void updateContent(const QVector<int> &roles = QVector<int>());

private:
	SceneTreeModel *m_sourceModel;
	QItemSelectionModel *m_sourceSelectionModel;
};


#endif // H_NODEPROPERTIESMODEL
