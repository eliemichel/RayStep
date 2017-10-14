#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow :
	public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void wheelEvent(QWheelEvent * event) override;

private slots:
	void showOpenFileDialog();

private:
	void fitImageSize();
	void updateImageSize();

private:
	Ui::MainWindow *ui;
	float m_scale;
};

