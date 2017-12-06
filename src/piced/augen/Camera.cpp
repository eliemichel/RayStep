#include "Camera.h"

#include <QDebug>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>


Camera::Camera()
	: m_sensitivity(0.003f)
	, m_panningSensitivity(0.01f)
	, m_zoomSensitivity(0.001f)
	, m_viewportWidth(640.f)
	, m_viewportHeight(480.f)
	, m_viewMatrix(1.f)
	, m_zoom(5.f)
	, m_center(0.f)
{
	m_quat = glm::quat(sqrt(2.f) / 2.f, -sqrt(2.f) / 2.f, 0.f, 0.f) * glm::quat(0.f, 0.f, 0.f, 1.f);

	glCreateBuffers(1, &m_ubo);
	glNamedBufferData(m_ubo, sizeof(m_viewMatrix) + sizeof(m_perspectiveMatrix), NULL, GL_DYNAMIC_DRAW);
	updateCameraViewMatrix();
	updateCameraProjectionMatrix();
}

Camera::~Camera()
{
	glDeleteBuffers(1, &m_ubo);
}

void Camera::mouseMoveRotation(float dx, float dy)
{
	// rotate around camera X axis by dy
	float theta = -dy * m_sensitivity;
	glm::vec3 xAxis = glm::vec3(glm::row(m_viewMatrix, 0));
	m_quat = m_quat * glm::quat(cos(theta / 2.f), sin(theta / 2.f) * xAxis);

	// rotate around world Z axis by dx
	theta = -dx * m_sensitivity;
	glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_quat = m_quat * glm::quat(cos(theta / 2.f), sin(theta / 2.f) * zAxis);

	updateCameraViewMatrix();
}

void Camera::mouseMovePanning(float dx, float dy)
{
	// rotate around camera X axis by dy
	glm::vec3 xAxis = glm::vec3(glm::row(m_viewMatrix, 0));
	glm::vec3 yAxis = glm::vec3(glm::row(m_viewMatrix, 1));

	m_center += -dx * m_panningSensitivity * xAxis + dy * m_panningSensitivity * yAxis;

	updateCameraViewMatrix();
}

void Camera::mouseScroll(float dx, float dy)
{
	m_zoom *= (1.f - dy * m_zoomSensitivity);
	m_zoom = std::max(m_zoom, 0.0001f);

	updateCameraViewMatrix();
}

void Camera::viewportResize(float w, float h)
{
	m_viewportWidth = w;
	m_viewportHeight = h;
	updateCameraProjectionMatrix();
}

void Camera::updateCameraViewMatrix()
{
	m_viewMatrix = glm::translate(glm::vec3(0.f, 0.f, -m_zoom)) * mat4_cast(m_quat) * glm::translate(m_center);

	glNamedBufferSubData(m_ubo, 0, sizeof(m_viewMatrix), glm::value_ptr(m_viewMatrix));
}

void Camera::updateCameraProjectionMatrix()
{
	m_perspectiveMatrix = glm::perspective(45.f, m_viewportWidth / m_viewportHeight, 0.1f, 100.f);

	glNamedBufferSubData(m_ubo, sizeof(m_viewMatrix), sizeof(m_perspectiveMatrix), glm::value_ptr(m_perspectiveMatrix));
}
