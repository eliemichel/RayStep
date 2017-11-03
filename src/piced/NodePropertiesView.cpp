#include "NodePropertiesView.h"
#include "NodeProperties.h"

NodePropertiesView::NodePropertiesView(QWidget *parent)
	: QAbstractItemView(parent)
{
	m_propertiesWidget = new NodeProperties(this);
}

NodePropertiesView::~NodePropertiesView()
{
}

QModelIndex NodePropertiesView::indexAt(const QPoint &point) const
{
	return currentIndex();
}

void NodePropertiesView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
	// noop
}

QRect NodePropertiesView::visualRect(const QModelIndex &index) const
{
	if (index == currentIndex())
	{
		return contentsRect();
	}
	else
	{
		return QRect();
	}
}

int NodePropertiesView::horizontalOffset() const
{
	return 0;
}

int NodePropertiesView::verticalOffset() const
{
	return 0;
}

bool NodePropertiesView::isIndexHidden(const QModelIndex &index) const
{
	return index != currentIndex();
}

QModelIndex NodePropertiesView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
	return currentIndex();
}

void NodePropertiesView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{
	// noop (selection is read only in this widget)
}

QRegion NodePropertiesView::visualRegionForSelection(const QItemSelection &selection) const
{
	if (selectedIndexes().contains(currentIndex()))
	{
		return contentsRect();
	}
	else
	{
		return QRect();
	}
}

