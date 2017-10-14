// **************************************************
// Author : Élie Michel <elie.michel@telecom-paristech.fr>
// UNPUBLISHED CODE.
// Copyright (C) 2017 Élie Michel.
// **************************************************

#pragma once

#include <glad/glad.h>

#ifdef _WIN32
#include <windows.h>
#else
#define APIENTRY
#endif

/**
 * Callback to use with glDebugMessageCallback
 * credits: https://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
 */
void APIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

#ifdef GLAD_DEBUG
/**
 * logs every gl call to the console
 */
void openglPreFunction(const char *name, void *funcptr, int len_args, ...);
#endif
