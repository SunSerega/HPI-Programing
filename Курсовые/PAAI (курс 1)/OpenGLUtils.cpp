#include "OpenGLUtils.h"

#include <fstream>
#include <string>

using namespace std;



GLuint InitShader(const char* fname, GLenum type) {
	auto f = ifstream{ fname };
	string text;

	f.seekg(0, std::ios::end);
	text.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	text.assign(
		std::istreambuf_iterator<char>{f},
		std::istreambuf_iterator<char>{}
	);

	auto res = glCreateShader(type);
	auto text_s = text.c_str();
	auto len = 1;
	glShaderSource(res, 1, &text_s, nullptr);
	glCompileShader(res);

	GLint status;
	glGetShaderiv(res, GL_COMPILE_STATUS, &status);
	if (!status) {

		GLint l;
		glGetShaderiv(res, GL_INFO_LOG_LENGTH, &l);

		auto log = new GLchar[l];
		glGetShaderInfoLog(res, l, nullptr, log);
		throw log;
		delete[] log;
	}

	return res;
}

GLuint InitProgram(initializer_list<GLuint> shaders) {
	auto res = glCreateProgram();

	for (auto shader : shaders)
		glAttachShader(res, shader);

	glLinkProgram(res);

	GLint status;
	glGetProgramiv(res, GL_LINK_STATUS, &status);
	if (!status) {

		GLint l;
		glGetProgramiv(res, GL_INFO_LOG_LENGTH, &l);

		auto log = new GLchar[l];
		glGetProgramInfoLog(res, l, nullptr, log);
		throw log;
		delete[] log;
	}

	return res;
}

GLint InitUniform(GLuint prog, const char* name)
{
	return glGetUniformLocation(prog, name);
}

GLint InitAttribute(GLuint prog, const char* name)
{
	return glGetAttribLocation(prog, name);
}

void ThrowIfErr()
{
	auto err = glGetError();
	if (err) throw err;
}


