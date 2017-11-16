
#include "Logger.h"
#include "Shader.h"
#include "ResourceManager.h"

#include "Actorio.h"

/// Struct shared with GLSL
#include <glm/glm.hpp>

Actorio::Actorio()
	: m_shader("render")
	, m_mouseMoveShader("mouse-move")
	, m_mouseScrollShader("mouse-scroll")
	, m_initShader("init")
	, m_updateShader("update")
	, m_isCameraRotating(false)
	, m_isCameraPanning(false)
{
	ShaderProgram::setRoot("E:/SourceCode/Piced/share/shaders");
	
	glGenVertexArrays(1, &m_vao);
	glCreateBuffers(1, &m_gameStateSsbo);
	glNamedBufferStorage(m_gameStateSsbo, sizeof(GameState), NULL, NULL);
	glCreateBuffers(1, &m_appInfoUbo);
	glNamedBufferData(m_appInfoUbo, sizeof(AppInfo), &m_appInfo, GL_DYNAMIC_DRAW);
	glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
	ResourceManager::loadTexture(m_texture, "");

	reloadShaders();
}

Actorio::~Actorio() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_gameStateSsbo);
	glDeleteBuffers(1, &m_appInfoUbo);
	glDeleteTextures(1, &m_texture);
}

void Actorio::reloadShaders() {
	LOG << "Loading shaders...";
	LOG << " - map";
	m_shader.load();
	m_shader.use();
	m_shader.bindUniformBlock("AppInfo", m_appInfoUbo, 1);
	m_shader.bindUniformBlock("GameState", m_gameStateSsbo, 2);  // bindUniformBlock -> const GameState method
	m_shader.bindUniformBlock("Camera", m_camera.ubo(), 3);
	m_shader.setUniform("iChannel1", 0);

	LOG << " - mouse move";
	m_mouseMoveShader.loadCompute();
	m_mouseMoveShader.use();
	m_mouseMoveShader.bindUniformBlock("AppInfo", m_appInfoUbo, 1);
	m_mouseMoveShader.bindStorageBlock("GameState", m_gameStateSsbo, 1);

	LOG << " - mouse scroll";
	m_mouseScrollShader.loadCompute();
	m_mouseScrollShader.use();
	m_mouseScrollShader.bindUniformBlock("AppInfo", m_appInfoUbo, 1);
	m_mouseScrollShader.bindStorageBlock("GameState", m_gameStateSsbo, 1);

	LOG << " - init";
	m_initShader.loadCompute();
	m_initShader.use();
	m_initShader.bindUniformBlock("AppInfo", m_appInfoUbo, 1);
	m_initShader.bindStorageBlock("GameState", m_gameStateSsbo, 1);

	LOG << " - update";
	m_updateShader.loadCompute();
	m_updateShader.use();
	m_updateShader.bindUniformBlock("AppInfo", m_appInfoUbo, 1);
	m_updateShader.bindStorageBlock("GameState", m_gameStateSsbo, 1);

	// Run init
	m_initShader.use();
	glDispatchCompute(1, 1, 1);
}

void Actorio::onKey(int key, int scancode, int action, int mod) {
	switch (action) {
	case PressKeyAction:
		switch (key) {
		case ReloadShaderKey:
			reloadShaders();
			break;
		}
		break;
	}
}

void Actorio::onCursorPos(double x, double y) {
	m_mouseMoveShader.use();
	m_mouseMoveShader.setUniform("mouse", glm::vec2(x, y));
	glDispatchCompute(1, 1, 1);
	//glMemoryBarrier(GL_ALL_BARRIER_BITS);  // not needed

	float dx = static_cast<float>(x - m_oldX);
	float dy = static_cast<float>(y - m_oldY);

	if (m_isCameraRotating) {
		m_camera.mouseMoveRotation(dx, dy);
	}
	if (m_isCameraPanning) {
		m_camera.mouseMovePanning(dx, dy);
	}

	m_oldX = x;
	m_oldY = y;
}

void Actorio::onMouseButton(int button, int action, int mods) {
	switch (button) {
	case LeftButton:
		m_isCameraRotating = action == PressButtonAction;
		break;

	case MiddleButton:
		m_isCameraPanning = action == PressButtonAction;
		break;
	}
}

void Actorio::onScroll(double xoffset, double yoffset) {
	m_mouseScrollShader.use();
	m_mouseScrollShader.setUniform("scroll", glm::vec2(xoffset, yoffset));
	glDispatchCompute(1, 1, 1);

	m_camera.mouseScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void Actorio::resize(int width, int height) {
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	m_appInfo.resolution.x = static_cast<float>(width);
	m_appInfo.resolution.y = static_cast<float>(height);
}

void Actorio::update(float time) {
	m_appInfo.time = static_cast<GLfloat>(time);

	m_updateShader.use();
	glDispatchCompute(1, 1, 1);
}

void Actorio::render() const {
	glClear(GL_COLOR_BUFFER_BIT);

	// Update common UBO
	glNamedBufferData(m_appInfoUbo, sizeof(AppInfo), &m_appInfo, GL_DYNAMIC_DRAW);

	m_shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void Actorio::updateShader(std::string renderSnippet, std::string extraUniforms) {
	m_shader.setSnippet("piced-scene", renderSnippet);
	m_shader.setSnippet("piced-uniforms", extraUniforms);
	reloadShaders();
}

void Actorio::setUniform(const std::string & name, GLfloat value) const {
	m_shader.use();
	m_shader.setUniform(name, value);
}
