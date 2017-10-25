#pragma once

#include <QWidget>

class SceneTree;
class SceneTreeModel;

namespace Ui {
	class TextWindow;
}

class TextWindow :
	public QWidget
{
	Q_OBJECT

public:
	explicit TextWindow(QWidget *parent = 0);
	~TextWindow();

	QString content() const;

public slots:
	void load(const QString & filename);

signals:
	void submitted();

private:
	Ui::TextWindow *ui;
	SceneTree *m_scene;
	SceneTreeModel *m_model;
};

