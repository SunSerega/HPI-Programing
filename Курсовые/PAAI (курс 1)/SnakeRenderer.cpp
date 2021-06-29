#include "SnakeRenderer.h"

struct SnakeFieldProgram : public ShaderProgram {

	enum {
		uniform_field_pos,
		uniform_cell_size,
		UNIFORM_COUNT
	};

	enum {
		attribute_prev_pos,
		attribute_curr_pos,
		attribute_next_pos,
		attribute_color,
		attribute_dir_bits,
		ATTRIBUTE_COUNT
	};

	SnakeFieldProgram()
		: ShaderProgram{
			ShaderStage{"SnakeField.vert", GL_VERTEX_SHADER},
			ShaderStage{"SnakeField.geom", GL_GEOMETRY_SHADER},
			ShaderStage{"Empty.frag", GL_FRAGMENT_SHADER},
		}
	{}

};

struct SnakeFieldBackgroundProgram : public ShaderProgram {

	enum {
		uniform_field_pos,
		uniform_field_size,
		UNIFORM_COUNT
	};

	SnakeFieldBackgroundProgram()
		: ShaderProgram{
			ShaderStage{"Empty.vert", GL_VERTEX_SHADER},
			ShaderStage{"SnakeFieldBackground.geom", GL_GEOMETRY_SHADER},
			ShaderStage{"Empty.frag", GL_FRAGMENT_SHADER},
		}
	{}

};

SnakeRenderer::SnakeRenderer()
	: field_program(new SnakeFieldProgram{})
	, field_background_program(new SnakeFieldBackgroundProgram{})
{}

void SnakeRenderer::SetFieldPos(Vec<2, double> pos, Vec<2, double> size, Vec<2, double> cell_size)
{

	field_background_program->SetUniform(SnakeFieldBackgroundProgram::uniform_field_pos, pos);
	field_background_program->SetUniform(SnakeFieldBackgroundProgram::uniform_field_size, size);

	field_program->SetUniform(SnakeFieldProgram::uniform_field_pos, pos);
	field_program->SetUniform(SnakeFieldProgram::uniform_cell_size, cell_size);

}

/**
#pragma pack(push, 1)
struct TempShaderOtp
{
	Vec<4, float> data[4];
};
#pragma pack(pop)
/**/

void SnakeRenderer::Flush()
{
	/**
	GLuint temp_otp;
	glCreateBuffers(1, &temp_otp);
	TempShaderOtp temp;
	glNamedBufferData(temp_otp, sizeof(TempShaderOtp), &temp, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, temp_otp);
	/**/

	{
		auto u = field_background_program->Use();
		glDrawArrays(GL_POINTS, 0, 1);
		//glGetNamedBufferSubData(temp_otp, 0, sizeof(TempShaderOtp), &temp);
	}

	{
		vertices.push_back(EmptyCell);
		auto u = field_program->Use(SnakeFieldProgram::ATTRIBUTE_COUNT);

		field_program->SetAttribute(SnakeFieldProgram::attribute_prev_pos, &vertices[0].pos, sizeof(SnakeCell));
		field_program->SetAttribute(SnakeFieldProgram::attribute_curr_pos, &vertices[1].pos, sizeof(SnakeCell));
		field_program->SetAttribute(SnakeFieldProgram::attribute_next_pos, &vertices[2].pos, sizeof(SnakeCell));
		field_program->SetAttribute(SnakeFieldProgram::attribute_color, &vertices[1].color, sizeof(SnakeCell));
		field_program->SetAttribute(SnakeFieldProgram::attribute_dir_bits, 1, &vertices[1].dir_bits, sizeof(SnakeCell));

		/**/
		glDrawArrays(GL_POINTS, 0, (GLsizei)vertices.size()-2);
		/*/
		glDrawArrays(GL_POINTS, 1, 1);
		ThrowIfErr();
		glGetNamedBufferSubData(temp_otp, 0, sizeof(TempShaderOtp), &temp);
		/**/

		vertices.clear();
		vertices.push_back(EmptyCell);
	}

}
