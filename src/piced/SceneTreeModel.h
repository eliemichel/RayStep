#pragma once

#include <QAbstractItemModel>

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
	static bool isRoot(const QModelIndex &indext);
};
