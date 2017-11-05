#ifndef H_SCENETREEVIEW
#define H_SCENETREEVIEW

#include <QTreeView>

class SceneTreeView : public QTreeView
{
	Q_OBJECT

public:
	explicit SceneTreeView(QWidget *parent = 0);
	~SceneTreeView();

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	void createAction();

private slots:
	void addNode();

private:
	QAction *m_addAction;
	QModelIndex m_ctxIndex;
};

#endif // H_SCENETREEVIEW
