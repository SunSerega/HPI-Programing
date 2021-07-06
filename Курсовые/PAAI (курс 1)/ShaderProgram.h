#pragma once

#include "glad/include/glad/gl.h"

#include <initializer_list>
#include <vector>

#include "Vec.h"
#include "OpenGLUtils.h"



class ShaderProgram {
private:
	GLuint id = 0;
	//const std::vector<ShaderStage> stages;

	struct UseLocker {
		GLuint id;
		GLint attribute_count;
		UseLocker(GLuint id, GLint attribute_count)
			: id(id), attribute_count(attribute_count)
		{
			glUseProgram(id);
			for (GLint i = 0; i < attribute_count; i++)
				glEnableVertexAttribArray(i);
		}
		UseLocker(UseLocker&& u) noexcept
			: id(u.id), attribute_count(u.attribute_count)
		{
			u.id = 0;
		}
		~UseLocker() {
			if (!id) return;
			for (GLint i = 0; i < attribute_count; i++)
				glDisableVertexAttribArray(i);
			glUseProgram(0);
		}
	};

protected:
	friend class ShaderStage;
	ShaderProgram(std::initializer_list<ShaderStage*> stages);
	template <class ... Ts> ShaderProgram(Ts&& ... stages) : ShaderProgram({ &stages... }) {};
	ShaderProgram(ShaderProgram&& prev) noexcept
		: id(prev.id)
	{
		prev.id = 0;
	}

public:
	virtual ~ShaderProgram();
	
	UseLocker Use(int attribute_count = 0) const {
		return { id, attribute_count };
	}

	void SetUniform(GLint id, double val) const {
		glProgramUniform1d(this->id, id, val);
	}
	void SetUniform(GLint id, const Vec<2, double>& val) const {
		glProgramUniform2dv(this->id, id, 1, &val[0]);
	}
	void SetUniform(GLint id, const Vec<3, double>& val) const {
		glProgramUniform3dv(this->id, id, 1, &val[0]);
	}
	void SetUniform(GLint id, const Vec<4, double>& val) const {
		glProgramUniform4dv(this->id, id, 1, &val[0]);
	}

	template <class T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	void SetAttribute(GLint id, GLint c, T* ptr, GLsizei stride = 0) const {
		glVertexAttribIPointer(id, c, GLTypeName<T>(), stride, ptr);
	}
	template <size_t c, class T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	void SetAttribute(GLint id, Vec<c, T>* ptr, GLsizei stride = 0) const {
		glVertexAttribIPointer(id, c, GLTypeName<T>(), stride, ptr);
	}

	template <class T, std::enable_if_t<!std::is_integral_v<T>, bool> = true>
	void SetAttribute(GLint id, GLint c, T* ptr, GLsizei stride = 0, GLboolean normalize = GL_FALSE) const {
		glVertexAttribPointer(id, c, GLTypeName<T>(), normalize, stride, ptr);
	}
	template <size_t c, class T, std::enable_if_t<!std::is_integral_v<T>, bool> = true>
	void SetAttribute(GLint id, Vec<c, T>* ptr, GLsizei stride = 0, GLboolean normalize = GL_FALSE) const {
		glVertexAttribPointer(id, c, GLTypeName<T>(), normalize, stride, ptr);
	}

};

class ShaderStage {
private:
	GLuint id;

	friend ShaderProgram::ShaderProgram(std::initializer_list<ShaderStage*>);

public:
	ShaderStage(const char* fname, GLenum type);
	ShaderStage(ShaderStage&& prev) noexcept
		: id(prev.id)
	{
		prev.id = 0;
	}
	~ShaderStage();

};


