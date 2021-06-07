#pragma once

#include "glad/include/glad/gl.h"
#include <string>



GLuint InitShader(const char* fname, GLenum type);
GLuint InitProgram(std::initializer_list<GLuint> shaders);

GLint InitUniform(GLuint prog, const char* name);
GLint InitAttribute(GLuint prog, const char* name);

void ThrowIfErr();

template<class T> GLenum GLTypeName() = delete;
template<> inline GLenum GLTypeName<int>() { return GL_INT; };
template<> inline GLenum GLTypeName<float>() { return GL_FLOAT; };
template<> inline GLenum GLTypeName<double>() { return GL_DOUBLE; };


