#include "SceneTreeView.h"
#include "SceneTreeModel.h"
#include "SceneGraph.h"
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

void SceneTreeView::setModel(QAbstractItemModel *model)
{
	ERR_LOG << "SceneTreeView only accepts SceneTreeModel objects as model";
}

void SceneTreeView::setModel(SceneTreeModel *model)
{
	QTreeView::setModel(model);
}

SceneTreeModel *SceneTreeView::sceneTreeModel()
{
	return static_cast<SceneTreeModel*>(model());
}

void SceneTreeView::createAction()
{
	m_addPrimitiveAction = new QAction(tr("Add primitive"), this);
	connect(m_addPrimitiveAction, &QAction::triggered, this, &SceneTreeView::addPrimitive);
	m_addOperationAction = new QAction(tr("Add operator"), this);
	connect(m_addOperationAction, &QAction::triggered, this, &SceneTreeView::addOperation);
}

void SceneTreeView::contextMenuEvent(QContextMenuEvent *event)
{
	m_ctxIndex = indexAt(event->pos());

	QMenu menu(this);
	menu.addAction(m_addPrimitiveAction);
	menu.addAction(m_addOperationAction);
	menu.exec(event->globalPos());
}

void SceneTreeView::addPrimitive()
{
	auto *node = new ScenePrimitiveNode();
	node->setName("New primitive");
	node->setSource("vec2( sdTorus(     pos-vec3( 0.0,0.25, 1.0), vec2(0.20,0.05) ), 25.0 )");
	sceneTreeModel()->insertExistingRow(node, m_ctxIndex.row(), m_ctxIndex.parent());
}

void SceneTreeView::addOperation()
{
	auto *node = new SceneOperationNode();
	node->setName("New operation");
	node->setOperation(SceneOperationNode::UnionOp);
	sceneTreeModel()->insertExistingRow(node, m_ctxIndex.row(), m_ctxIndex.parent());
}
