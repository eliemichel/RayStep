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
	void updateCameraViewMatrix();

	inline GLuint ubo() const { return m_ubo; }

private:
	float m_sensitivity;
	glm::mat4 m_viewMatrix, m_perspectiveMatrix;
	glm::quat m_quat;
	float m_zoom;
	glm::vec3 m_center;
	GLuint m_ubo;
};

