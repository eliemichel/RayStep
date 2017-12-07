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
		LeftButton,
		MiddleButton,
		RightButton,
		UnknownButton
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

	void updateShader(std::string renderSnippet, std::string extraUniforms);

	// FOrward of Shader's setUniform. Maybe not a good design to keep this here but I add it temporarily to wire up uniform update behavior
	void setUniform(const std::string & name, GLfloat value) const;

private:
	void reloadShaders();

private:
	ShaderProgram m_raymarchingShader, m_manipulatorShader, m_mouseMoveShader, m_mouseScrollShader, m_initShader, m_updateShader;
	GLuint m_texture;
	GLuint m_raymarchingVao, m_manipulatorVao, m_manipulatorVbo;
	GLuint m_gameStateSsbo;
	AppInfo m_appInfo;
	GLuint m_appInfoUbo;
	Camera m_camera;
	double m_oldX, m_oldY;
	bool m_isCameraRotating, m_isCameraPanning;
	int m_nbManipulatorIndices;
	bool m_renderManipulator; // TODO: add accessors
};
