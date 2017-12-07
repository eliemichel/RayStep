// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

#if _DEBUG
#include "ShaderPreprocessor.h"
#endif

/**
 * Utility class providing an OO API to OpenGL shaders
 */
class Shader {
public:
    Shader(GLenum shaderType = 0);
    ~Shader();

    /**
     * Load file into the shader
     * @param filename Shader file
     */
    bool load(const std::string &filename, const std::vector<std::string> & defines = {}, const std::map<std::string, std::string> & snippets = {});

    /**
     * Compile the shader
     */
    inline void compile() { glCompileShader(m_shaderId); }

    /**
     * Check that the shader has been successfully compiled
     * @param name Name displayed in error message
     * @param success status
     */
    bool check(const std::string & name = "shader") const;

    inline GLuint shaderId() const { return m_shaderId; }

private:
    GLuint m_shaderId;

#if _DEBUG
	// Keep shader source
	ShaderPreprocessor m_preprocessor;
#endif
};


/**
 * Utility class providing an OO API to OpenGL shader program
 */
class ShaderProgram {
public:
	inline static void setRoot(const std::string & root) { s_root = root; }
	inline static const std::string & root() { return s_root; }

private:
	static std::string s_root;

public:
	ShaderProgram(const std::string & shaderName = "");
	ShaderProgram(ShaderProgram&&) = default;
	ShaderProgram& operator=(ShaderProgram&&) = default;

	/**
	 * NB: Changing shader name does not reload it. You may want to call load() then.
	 */
	inline void setShaderName(const std::string & shaderName) { m_shaderName = shaderName; }
	inline const std::string & shaderName() const { return m_shaderName; }

	inline void define(const std::string & def) { m_defines.insert(def); }
	inline void undefine(const std::string & def) { m_defines.erase(def); }

	inline void setSnippet(const std::string & key, const std::string & value) { m_snippets[key] = value; }

    /**
     * Load and check shaders
     */
	void load();

	/**
	* Load and check a compute shader program
	*/
	void loadCompute();

    /**
     * Check that the shader program has been successfully compiled
     * @param name Name displayed in error message
     * @param success status
     */
    bool check(const std::string & name = "shader program") const;

    inline void use() const { if (isValid()) glUseProgram(m_programId); }
	inline bool isValid() const { return m_isValid; }

    // Some overloading: (add whatever you need)
    void setUniform(const std::string & name, GLint value) const;
	void setUniform(const std::string & name, GLuint value) const;
	void setUniform(const std::string & name, GLfloat value) const;
	void setUniform(const std::string & name, const glm::vec3 & value) const;
	void setUniform(const std::string & name, const glm::vec2 & value) const;
    void setUniform(const std::string & name, const glm::mat3 & value) const;
	void setUniform(const std::string & name, const glm::mat4 & value) const;

	bool bindUniformBlock(const std::string & blockName, GLuint buffer, GLuint uniformBlockBinding = 1);
	bool bindStorageBlock(const std::string & blockName, GLuint buffer, GLuint uniformBlockBinding = 1);

private:
	std::string m_shaderName;
	std::set<std::string> m_defines;
	std::map<std::string, std::string> m_snippets;
	std::vector<std::unique_ptr<Shader>> m_shaders;
    GLuint m_programId;
	bool m_isValid;

private:
    inline GLint uniformLocation(const std::string & name) const { return m_isValid ? glGetUniformLocation(m_programId, name.c_str()) : -1; }
	inline GLuint uniformBlockIndex(const std::string & name) const { return m_isValid ? glGetUniformBlockIndex(m_programId, name.c_str()) : GL_INVALID_INDEX; }
	inline GLuint storageBlockIndex(const std::string & name) const { return m_isValid ? glGetProgramResourceIndex(m_programId, GL_SHADER_STORAGE_BLOCK, name.c_str()) : GL_INVALID_INDEX; }
};
