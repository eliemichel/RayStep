#pragma once

#include <QWidget>

class Actorio;

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

	Actorio *actorio() const;

private:
	Ui::ViewportWindow *ui;
};

