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

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private slots:
	void render();

private:
	static int Q2AButton(Qt::MouseButton qButton);

private:
	Actorio *m_actorio;
	float m_time;
	QTimer *m_renderTimer;
	bool m_isRotation;
};
