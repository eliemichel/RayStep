#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	~Camera();
	void mouseMoveRotation(float dx, float dy);
	void mouseMovePanning(float dx, float dy);
	void mouseScroll(float dx, float dy);
	void viewportResize(float w, float h);
	void updateCameraViewMatrix();
	void updateCameraProjectionMatrix();

	inline GLuint ubo() const { return m_ubo; }

private:
	float m_sensitivity, m_panningSensitivity, m_zoomSensitivity;
	float m_viewportWidth, m_viewportHeight;
	glm::mat4 m_viewMatrix, m_perspectiveMatrix;
	glm::quat m_quat;
	float m_zoom;
	glm::vec3 m_center;
	GLuint m_ubo;
};

