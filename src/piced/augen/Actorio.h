#include "Shader.h"
#include "Camera.h"

/// Struct shared with GLSL
#include <glm/glm.hpp>

// State written and read from GPU only
struct GameState {
	glm::vec4 mouse;
	glm::vec4 cameraPosition;
	GLfloat cameraZoom;
	GLfloat _pad[3];
};

// State written from CPU and read from GPU
struct AppInfo {
	glm::vec2 resolution;
	GLfloat time;
	GLfloat _pad;
};

/// Main window class
class Actorio {
public:
	enum Key {
		ReloadShaderKey
	};
	enum KeyAction {
		PressKeyAction,
		ReleaseKeyAction
	};
	enum Button {
		LeftButton
	};
	enum ButtonAction {
		PressButtonAction,
		ReleaseButtonAction
	};

public:
	Actorio();
	~Actorio();
	void update(float time);
	void render() const;
	void resize(int width, int height);

	void onKey(int key, int scancode, int action, int mod);
	void onCursorPos(double x, double y);
	void onMouseButton(int button, int action, int mods);
	void onScroll(double xoffset, double yoffset);

private:
	void reloadShaders();

private:
	ShaderProgram m_shader, m_mouseMoveShader, m_mouseScrollShader, m_initShader, m_updateShader;
	GLuint m_texture;
	GLuint m_vao;
	GLuint m_gameStateSsbo;
	AppInfo m_appInfo;
	GLuint m_appInfoUbo;
	Camera m_camera;
	float m_oldX, m_oldY;
	bool m_isCameraRotating;
};
