#ifndef H_SCENETREEVIEW
#define H_SCENETREEVIEW

#include <QTreeView>

class SceneTreeModel;

class SceneTreeView : public QTreeView
{
	Q_OBJECT

public:
	explicit SceneTreeView(QWidget *parent = 0);
	~SceneTreeView();

	void setModel(QAbstractItemModel *model) override;
	void setModel(SceneTreeModel *model);
	SceneTreeModel *sceneTreeModel();

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	void createAction();

private slots:
	void addPrimitive();
	void addOperation();

private:
	QAction *m_addPrimitiveAction;
	QAction *m_addOperationAction;
	QModelIndex m_ctxIndex;
};

#endif // H_SCENETREEVIEW
