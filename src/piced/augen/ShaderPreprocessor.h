// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#pragma once

#include "glad/glad.h"
#include <string>
#include <vector>

class ShaderPreprocessor {
public:
	/**
	 * Load a shader from a file, and reccursively include #include'd files
	 * You can specity a list of defines to #define where ever
	 * `#include "sys:defines"` is found in the shader.
	 */
	bool load(const std::string & filename, const std::vector<std::string> & defines = {});

	/**
	 * Return a buffer to the pure GLSL source, post-preprocessing, to be fed into
	 * glShaderSource.
	 */
	void source(std::vector<GLchar> & buf) const;

	/**
	 * Log a traceback corresponding to the line `line` in the processed source.
	 */
	void logTraceback(size_t line) const;

private:
	std::vector<std::string> m_lines;

private:
	static bool loadShaderSourceAux(const std::string & filename, const std::vector<std::string> & defines, std::vector<std::string> & lines_accumulator);
};

