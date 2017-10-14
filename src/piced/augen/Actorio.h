#include "Shader.h"
#include "GenericApp.h"

/// Struct shared with GLSL
#include <glm/glm.hpp>

// State written and read from GPU only
struct GameState {
	glm::vec4 mouse;
	glm::vec2 cameraPosition;
	GLfloat cameraZoom;
	GLfloat _pad;
};

// State written from CPU and read from GPU
struct AppInfo {
	glm::vec2 resolution;
	GLfloat time;
	GLfloat _pad;
};

/// Main window class
class Actorio : public GenericApp {
public:
	Actorio();
	~Actorio();
	void update(float time) override;
	void render() const override;

protected:
	void onKey(int key, int scancode, int action, int mod) override;
	void onCursorPos(double x, double y) override;
	void onMouseButton(int button, int action, int mods) override;
	void onScroll(double xoffset, double yoffset) override;
	void onFramebufferSize(int width, int height) override;

private:
	void reloadShaders();

private:
	ShaderProgram m_shader, m_mouseMoveShader, m_mouseScrollShader, m_initShader, m_updateShader;
	GLuint m_texture;
	GLuint m_vao;
	GLuint m_gameStateSsbo;
	AppInfo m_appInfo;
	GLuint m_appInfoUbo;
};
