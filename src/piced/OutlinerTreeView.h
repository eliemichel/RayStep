#ifndef H_OUTLINERTREEVIEW
#define H_OUTLINERTREEVIEW

#include <QTreeView>

class OutlinerTreeView : public QTreeView
{
public:
	explicit OutlinerTreeView(QWidget *parent = 0);
	~OutlinerTreeView();
};

#endif // H_OUTLINERTREEVIEW
