#pragma once

#include "glad/include/glad/gl.h"

#include <vector>
#include <memory>

#include "OpenGLUtils.h"
#include "Vec.h"
#include "ShaderProgram.h"



class SnakeRenderer{
	Vec<2, int> field_size;
	std::unique_ptr<ShaderProgram> field_program;
	std::unique_ptr<ShaderProgram> field_background_program;

	struct SnakeCell {
		Vec<2, int> pos;
		Vec<3, float> color;
		Vec<2, int> dir[2];

		SnakeCell(Vec<2, int> pos, Vec<3, float> color)
			: pos{ pos }, color{ color }, dir{}
		{}

		SnakeCell(Vec<2, int> pos, Vec<3, float> color, Vec<2, int> dir1)
			: pos{ pos }, color{ color }, dir{ dir1, {} }
		{}

	};

	std::vector<SnakeCell> vertices{};

public:
	SnakeRenderer(Vec<2, int> field_size);
	SnakeRenderer(SnakeRenderer&& prev) noexcept
		: field_size{ prev.field_size }
		, field_program{ std::move(prev.field_program) }
		, field_background_program{ std::move(prev.field_background_program) }
	{}

	void SetFieldPos(Vec<2, double> pos, Vec<2, double> size, double cell_size);

	void AddHead(Vec<2, int> pos, Vec<3, float> color) {
		vertices.push_back({ pos, color });
	}

	void AddBody(Vec<2, int> pos, Vec<3, float> color) {
		auto& prev = vertices.back();
		auto dir = pos - prev.pos;
		prev.dir[1] = dir;
		vertices.push_back({ pos, color, dir });
	}

	void AddFood(Vec<2, int> pos) {
		vertices.push_back({ pos, Vec<3, float>{0,0,1} });
	}

	void Flush();

};


