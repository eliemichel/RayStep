#ifndef H_NODEPROPERTIES
#define H_NODEPROPERTIES

#include <QWidget>
#include <QModelIndex>

class SceneTreeModel;
class QItemSelectionModel;

namespace Ui {
	class NodeProperties;
}

class NodeProperties :
	public QWidget
{
	Q_OBJECT

public:
	explicit NodeProperties(QWidget *parent = 0);
	~NodeProperties();

	SceneTreeModel *model() const { return m_model; }
	void setModel(SceneTreeModel *model);

	QItemSelectionModel *selectionModel() const { return m_selectionModel; }
	void setSelectionModel(QItemSelectionModel *selectionModel);

private slots:
	void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
	void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
	void updateContent(const QVector<int> &roles = QVector<int>());

	void onNameChanged(const QString &name);

private:
	Ui::NodeProperties *ui;
	SceneTreeModel *m_model;
	QItemSelectionModel *m_selectionModel;
};

#endif // H_NODEPROPERTIES
