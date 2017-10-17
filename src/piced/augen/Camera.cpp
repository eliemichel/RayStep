#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
	: m_sensitivity(0.003f)
	, m_viewMatrix(1.f)
	, m_zoom(6.f)
	, m_center(0.f)
{
	m_perspectiveMatrix = glm::perspective(45.f, 640.f / 480.f, 0.1f, 100.f);

	glCreateBuffers(1, &m_ubo);
	glNamedBufferData(m_ubo, sizeof(m_viewMatrix) + sizeof(m_perspectiveMatrix), NULL, GL_DYNAMIC_DRAW);
	updateCameraViewMatrix();
	glNamedBufferSubData(m_ubo, sizeof(m_viewMatrix), sizeof(m_perspectiveMatrix), glm::value_ptr(m_perspectiveMatrix));
}

Camera::~Camera()
{
	glDeleteBuffers(1, &m_ubo);
}

void Camera::mouseMoveRotation(float dx, float dy)
{
	// rotate around camera X axis by dy
	float theta = dy * m_sensitivity;
	glm::vec3 xAxis = glm::vec3(m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0]);
	m_quat = m_quat * glm::quat(theta, xAxis);

	// rotate around world Z axis by dx
	theta = dx * m_sensitivity;
	glm::vec3 zAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	m_quat = m_quat * glm::quat(theta, zAxis);

	updateCameraViewMatrix();
}

void Camera::updateCameraViewMatrix()
{
	m_viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -m_zoom));
	m_viewMatrix = m_viewMatrix * mat4_cast(m_quat);
	m_viewMatrix = glm::translate(m_viewMatrix, m_center);

	glNamedBufferData(m_ubo, sizeof(m_viewMatrix), glm::value_ptr(m_viewMatrix), GL_DYNAMIC_DRAW);
}
