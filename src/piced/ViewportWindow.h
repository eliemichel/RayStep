#pragma once

#include <QWidget>

namespace Ui {
	class ViewportWindow;
}

class ViewportWindow :
	public QWidget
{
	Q_OBJECT

public:
	explicit ViewportWindow(QWidget *parent = 0);
	~ViewportWindow();

private:
	Ui::ViewportWindow *ui;
};

