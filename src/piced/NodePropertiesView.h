#ifndef H_NODEPROPERTIESVIEW
#define H_NODEPROPERTIESVIEW

#include <QAbstractItemView>

class NodeProperties;

class NodePropertiesView : public QAbstractItemView
{
public:
	explicit NodePropertiesView(QWidget *parent = 0);
	~NodePropertiesView();

	virtual QModelIndex indexAt(const QPoint &point) const override;
	virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
	virtual QRect visualRect(const QModelIndex &index) const override;

protected:
	virtual int horizontalOffset() const override;
	virtual int verticalOffset() const override;
	virtual bool isIndexHidden(const QModelIndex &index) const override;
	virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
	virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags) override;
	virtual QRegion visualRegionForSelection(const QItemSelection &selection) const override;

private:
	NodeProperties *m_propertiesWidget;
};

#endif // H_NODEPROPERTIESVIEW
