#include <glad/glad.h>

#include "RenderWidget.h"
#include "augen/Actorio.h"

#include <QDebug>

RenderWidget::RenderWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, m_actorio(nullptr)
	, m_time(0.0f)
{
	qDebug() << "Yo OpenGL";
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
	
	qDebug() << "Render OpenGL view";
	m_actorio->update(m_time);
	m_actorio->render();
}
