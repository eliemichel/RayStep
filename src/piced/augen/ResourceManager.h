#pragma once

#include <glad/glad.h>
#include <string>

class ResourceManager {
public:
	static bool loadTexture(GLuint tex, const std::string & filename, GLenum internalformat = GL_RGBA8);
	static bool loadTexture(GLuint tex, const unsigned char* data, size_t length, GLenum internalformat = GL_RGBA8);
};


