#include "SceneTreeView.h"
#include "Logger.h"

#include <QContextMenuEvent>
#include <QMenu>

SceneTreeView::SceneTreeView(QWidget *parent)
	: QTreeView(parent)
{
	createAction();
}

SceneTreeView::~SceneTreeView()
{
}

void SceneTreeView::createAction()
{
	m_addAction = new QAction(tr("&Add node"), this);
	connect(m_addAction, &QAction::triggered, this, &SceneTreeView::addNode);
}

void SceneTreeView::contextMenuEvent(QContextMenuEvent *event)
{
	m_ctxIndex = indexAt(event->pos());

	QMenu menu(this);
	menu.addAction(m_addAction);
	menu.exec(event->globalPos());
}

void SceneTreeView::addNode()
{
	DEBUG_LOG << "m_ctxIndex: " << m_ctxIndex.row() << ", " << m_ctxIndex.column() << ", " << m_ctxIndex.internalPointer();
	model()->insertRows(m_ctxIndex.row(), 1, m_ctxIndex.parent());
}
