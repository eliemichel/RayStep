// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#include "Shader.h"
#include "utils/fileutils.h"
#include "utils/strutils.h"
#include "ShaderPreprocessor.h"
#include "Logger.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

Shader::Shader(GLenum shaderType)
    : m_shaderId(0)
{
	m_shaderId = glCreateShader(shaderType);
}


Shader::~Shader() {
    glDeleteShader(m_shaderId);
}

bool Shader::load(const string &filename, const vector<string> & defines, const std::map<std::string, std::string> & snippets) {
	ShaderPreprocessor preprocessor;

	if (!preprocessor.load(filename, defines, snippets)) {
		return false;
	}

	vector<GLchar> buf;
	preprocessor.source(buf);
	const GLchar *source = &buf[0];
    glShaderSource(m_shaderId, 1, &source, 0);

#if _DEBUG
	m_preprocessor = preprocessor;
#endif

	return true;
}


bool Shader::check(const string & name) const {
    int ok;
    
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        int len;
        glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &len);
        char *log = new char[len];
        glGetShaderInfoLog(m_shaderId, len, &len, log);

		ERR_LOG << "Unable to compile " << name << ". OpenGL returned:";
		// Example of error message:
		// 0(158) : error C0000: syntax error, unexpected '/' at token "/"
		std::istringstream iss(log);
		size_t line, column;
		char tmp;
		string msg, l;
		while (getline(iss, l)) {
			std::istringstream lss(l);
			lss >> column >> tmp >> line;
			getline(lss, msg);
			ERR_LOG << msg.substr(4);
#ifdef _DEBUG
			m_preprocessor.logTraceback(line);
#endif
		}

		delete[] log;
    }

    return ok;
}


// **************************************************
// ShaderProgram
// **************************************************

std::string ShaderProgram::s_root = "D:\\Master\\Src\\Augen\\share\\shaders";

ShaderProgram::ShaderProgram(const std::string & shaderName)
    : m_shaderName(shaderName)
	, m_isValid(false)
{}

void ShaderProgram::load() {
	m_programId = glCreateProgram();

	vector<string> defines(m_defines.begin(), m_defines.end());

	m_shaders.resize(0);
	m_shaders.reserve(3);

	std::unique_ptr<Shader> vertexShader(new Shader(GL_VERTEX_SHADER));
	vertexShader->load(fixPath(joinPath(s_root, m_shaderName + ".vert.glsl")), defines, m_snippets);
    vertexShader->compile();
    vertexShader->check("vertex shader");
	glAttachShader(m_programId, vertexShader->shaderId());
	m_shaders.push_back(std::move(vertexShader));

	std::unique_ptr<Shader> geometryShader(new Shader(GL_GEOMETRY_SHADER));
	if (geometryShader->load(fixPath(joinPath(s_root, m_shaderName + ".geo.glsl")), defines, m_snippets)) {
		geometryShader->compile();	
		geometryShader->check("geometry shader");
		glAttachShader(m_programId, geometryShader->shaderId());
		m_shaders.push_back(std::move(geometryShader));
	}

	std::unique_ptr<Shader> fragmentShader(new Shader(GL_FRAGMENT_SHADER));
	fragmentShader->load(fixPath(joinPath(s_root, m_shaderName + ".frag.glsl")), defines, m_snippets);
    fragmentShader->compile();
    fragmentShader->check("fragment shader");
	glAttachShader(m_programId, fragmentShader->shaderId());
	m_shaders.push_back(std::move(fragmentShader));

    glLinkProgram(m_programId);
	m_isValid = check();
}


void ShaderProgram::loadCompute() {
	m_programId = glCreateProgram();

	vector<string> defines(m_defines.begin(), m_defines.end());

	m_shaders.resize(0);
	m_shaders.reserve(1);

	std::unique_ptr<Shader> computeShader(new Shader(GL_COMPUTE_SHADER));
	computeShader->load(fixPath(joinPath(s_root, m_shaderName + ".comp.glsl")), defines, m_snippets);
	computeShader->compile();
	computeShader->check("compute shader");
	glAttachShader(m_programId, computeShader->shaderId());
	m_shaders.push_back(std::move(computeShader));

	glLinkProgram(m_programId);
	m_isValid = check();
}


bool ShaderProgram::check(const string & name) const {
    int ok;
    
    glGetProgramiv(m_programId, GL_LINK_STATUS, &ok);
    if (!ok) {
        int len;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &len);
        char *log = new char[len];
        glGetProgramInfoLog(m_programId, len, &len, log);
        cerr
            << "ERROR: Unable to link " << name << " (" << m_shaderName << "). OpenGL returned:" << endl
            << log << endl << endl;
        delete [] log;
    }

    return ok;
}


void ShaderProgram::setUniform(const string & name, GLint value) const {
    glUniform1i(uniformLocation(name), value);
}
void ShaderProgram::setUniform(const string & name, GLuint value) const {
	glUniform1ui(uniformLocation(name), value);
}
void ShaderProgram::setUniform(const string & name, GLfloat value) const {
	glUniform1f(uniformLocation(name), value);
}
void ShaderProgram::setUniform(const string & name, const glm::vec3 & value) const {
	glUniform3f(
		uniformLocation(name),
		static_cast<GLfloat>(value.x),
		static_cast<GLfloat>(value.y),
		static_cast<GLfloat>(value.z));
}
void ShaderProgram::setUniform(const string & name, const glm::vec2 & value) const {
	glUniform2f(
		uniformLocation(name),
		static_cast<GLfloat>(value.x),
		static_cast<GLfloat>(value.y));
}
void ShaderProgram::setUniform(const string & name, const glm::mat3 & value) const {
	glUniformMatrix3fv(uniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderProgram::setUniform(const string & name, const glm::mat4 & value) const {
	glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

bool ShaderProgram::bindUniformBlock(const std::string & blockName, GLuint buffer, GLuint uniformBlockBinding) {
	GLuint index = uniformBlockIndex(blockName);
	if (index == GL_INVALID_INDEX) {
		WARN_LOG << "Uniform Block not found with name " << blockName;
		return false;
	}
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockBinding, buffer);
	glUniformBlockBinding(m_programId, index, uniformBlockBinding);
	return true;
}

bool ShaderProgram::bindStorageBlock(const std::string & blockName, GLuint buffer, GLuint uniformBlockBinding) {
	GLuint index = storageBlockIndex(blockName);
	if (index == GL_INVALID_INDEX) {
		WARN_LOG << "Storage Block not found with name " << blockName;
		return false;
	}
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, uniformBlockBinding, buffer);
	glShaderStorageBlockBinding(m_programId, index, uniformBlockBinding);
	return true;
}
