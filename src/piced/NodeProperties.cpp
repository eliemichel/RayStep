#include "NodeProperties.h"

NodeProperties::NodeProperties(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::TextWindow)
{
	ui->setupUi(this);
}

NodeProperties::~NodeProperties()
{
}
