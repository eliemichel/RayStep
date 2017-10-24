#pragma once

#include <QAbstractItemModel>

class SceneTree;

class SceneTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	explicit SceneTreeModel(QObject *parent = Q_NULLPTR);
	~SceneTreeModel();

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
	SceneTree *sceneTreeAt(const QModelIndex &index) const;

private:
	static bool isRoot(const QModelIndex &indext);

private:
	SceneTree *m_dataTree;
};
