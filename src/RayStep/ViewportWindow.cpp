#include "ViewportWindow.h"
#include "ui_ViewportWindow.h"

#include <QDebug>

ViewportWindow::ViewportWindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::ViewportWindow)
{
	ui->setupUi(this);
}

ViewportWindow::~ViewportWindow()
{
}

Actorio *ViewportWindow::actorio() const
{
	return ui->render->actorio();
}
