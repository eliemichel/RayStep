#include <glad/glad.h>

#include "RenderWidget.h"
#include "augen/Actorio.h"

#include <QDebug>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>

RenderWidget::RenderWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, m_actorio(nullptr)
	, m_time(0.0f)
	, m_renderTimer(new QTimer(this))
{
	connect(m_renderTimer, SIGNAL(timeout()), this, SLOT(render()));
	m_renderTimer->start(20);
	qDebug() << "Yo OpenGL";

	setMouseTracking(true);
}

void RenderWidget::initializeGL()
{
	if (gladLoadGL()) {
		m_actorio = new Actorio;
	}
	qDebug() << "OpenGL status: " << (m_actorio != nullptr);
}

void RenderWidget::resizeGL(int w, int h)
{
	if (!m_actorio)
		return;

	m_actorio->resize(w, h);
}

void RenderWidget::paintGL()
{
	if (!m_actorio)
		return;

	m_time += 1.0f;
	
	m_actorio->update(m_time);
	m_actorio->render();
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
	if (!m_actorio)
		return;

	int button = Q2AButton(event->button());
	int mods = 0; // TODO

	m_actorio->onMouseButton(button, Actorio::PressButtonAction, mods);
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (!m_actorio)
		return;

	int button = Q2AButton(event->button());
	int mods = 0; // TODO

	m_actorio->onMouseButton(button, Actorio::ReleaseButtonAction, mods);
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_actorio)
		return;

	QPointF p = event->localPos();
	m_actorio->onCursorPos(static_cast<double>(p.x()), static_cast<double>(p.y()));
}

void RenderWidget::wheelEvent(QWheelEvent *event)
{
	if (!m_actorio)
		return;

	QPoint p = event->angleDelta();
	m_actorio->onScroll(static_cast<double>(p.x()), static_cast<double>(p.y()));
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{
	if (!m_actorio)
		return;

	int scancode = 0; // TODO
	int mod = 0; // TODO
	switch (event->key())
	{
	case Qt::Key_F5:
		m_actorio->onKey(Actorio::ReloadShaderKey, scancode, Actorio::PressKeyAction, mod);
		break;
	}
}

void RenderWidget::render()
{
	update();
}





int RenderWidget::Q2AButton(Qt::MouseButton qButton) {
	switch (qButton) {
	case Qt::LeftButton:
		return Actorio::LeftButton;
		break;
	case Qt::MiddleButton:
		return Actorio::MiddleButton;
		break;
	case Qt::RightButton:
		return Actorio::RightButton;
		break;
	default:
		return Actorio::UnknownButton;
	}
}
