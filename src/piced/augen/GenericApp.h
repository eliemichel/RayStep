#pragma once

#include <glad/glad.h>

#include "Logger.h"

class GenericApp {
public:
	GenericApp() : m_isValid(false) {}
	inline bool isValid() const { return m_isValid; }
	virtual void update(float time) {}
	virtual void render() const {}
	void resize(int width, int height) {
		onWindowSize(width, height);
		onFramebufferSize(width, height);
	}

protected:
	virtual void onKey(int key, int scancode, int action, int mod) {}
	virtual void onCursorPos(double x, double y) {}
	virtual void onCursorEnter(int entered) {}
	virtual void onMouseButton(int button, int action, int mods) {}
	virtual void onScroll(double xoffset, double yoffset) {}
	virtual void onWindowClose() {}
	virtual void onWindowSize(int width, int height) {}
	virtual void onFramebufferSize(int width, int height) {}
	virtual void onWindowPos(int x, int y) {}
	virtual void onWindowIconify(int iconified) {}
	virtual void onWindowFocus(int focused) {}
	virtual bool init() { return true; }

private:
	bool m_isValid;
};
