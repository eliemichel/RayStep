#ifndef H_NODEPROPERTIES
#define H_NODEPROPERTIES

#include <QWidget>

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

private:
	Ui::NodeProperties *ui;
};

#endif // H_NODEPROPERTIES
