#pragma once

#include <QOpenGLWidget>

class Actorio;

class RenderWidget : public QOpenGLWidget
{
	Q_OBJECT;
public:
	explicit RenderWidget(QWidget *parent = 0);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	Actorio *m_actorio;
	float m_time;
};
