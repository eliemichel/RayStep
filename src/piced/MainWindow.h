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

private slots:
	void showOpenFileDialog();
	void updateShader();
	void updateUniform(std::string name, float value);

private:
	Ui::MainWindow *ui;
	float m_scale;
};

