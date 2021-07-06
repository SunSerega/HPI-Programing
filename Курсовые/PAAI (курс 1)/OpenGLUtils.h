#pragma once

#include "glad/include/glad/gl.h"



void ThrowIfErr();

template<class T> GLenum GLTypeName() = delete;
template<> inline GLenum GLTypeName<int>() { return GL_INT; };
template<> inline GLenum GLTypeName<float>() { return GL_FLOAT; };
template<> inline GLenum GLTypeName<double>() { return GL_DOUBLE; };


