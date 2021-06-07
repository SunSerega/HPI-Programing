#include "ShaderProgram.h"

#include <string>
#include <fstream>

using namespace std;


ShaderStage::ShaderStage(const char* fname, GLenum type)
	: id( glCreateShader(type) )
{
	auto f = ifstream{ fname };
	string text;

	f.seekg(0, std::ios::end);
	text.reserve(f.tellg());
	f.seekg(0, std::ios::beg);

	text.assign(
		std::istreambuf_iterator<char>{f},
		std::istreambuf_iterator<char>{}
	);

	auto text_s = text.c_str();
	glShaderSource(id, 1, &text_s, nullptr);
	glCompileShader(id);

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (!status) {

		GLint l;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);

		auto log = new GLchar[l];
		glGetShaderInfoLog(id, l, nullptr, log);
		throw log;
		delete[] log;
	}

}
ShaderStage::~ShaderStage()
{
	if (!id) return;
	glDeleteShader(id);
}

ShaderProgram::ShaderProgram(std::initializer_list<ShaderStage> stages)
	: id( glCreateProgram() )
	//, stages(stages)
{
	for (auto&& stage : stages)
		glAttachShader(this->id, stage.id);
	glLinkProgram(id);

	GLint status;
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (!status) {

		GLint l;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);

		auto log = new GLchar[l];
		glGetProgramInfoLog(id, l, nullptr, log);
		throw log;
		delete[] log;
	}

}
ShaderProgram::~ShaderProgram()
{
	if (!id) return;
	glDeleteProgram(id);
}
