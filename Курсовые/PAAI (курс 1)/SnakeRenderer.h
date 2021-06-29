#pragma once

#include "glad/include/glad/gl.h"

#include <vector>
#include <memory>

#include "OpenGLUtils.h"
#include "Vec.h"
#include "ShaderProgram.h"



class SnakeRenderer{
	std::unique_ptr<ShaderProgram> field_program;
	std::unique_ptr<ShaderProgram> field_background_program;

	struct SnakeCell {
		Vec<2, int> pos;
		Vec<3, float> color;
		int dir_bits;
		static constexpr int DIR_HAS_PREV_BIT = 0b01;
		static constexpr int DIR_HAS_NEXT_BIT = 0b10;
	};

	static constexpr SnakeCell EmptyCell{ {},{},0 };
	std::vector<SnakeCell> vertices{ EmptyCell };

public:
	SnakeRenderer();
	SnakeRenderer(SnakeRenderer&& prev) noexcept
		: field_program{ std::move(prev.field_program) }
		, field_background_program{ std::move(prev.field_background_program) }
	{}

	void SetFieldPos(Vec<2, double> pos, Vec<2, double> size, Vec<2, double> cell_size);

	void AddHead(Vec<2, int> pos, Vec<3, float> color) {
		vertices.push_back({ pos, color, 0 });
	}

	void AddBody(Vec<2, int> pos, Vec<3, float> color) {
		vertices.back().dir_bits |= SnakeCell::DIR_HAS_NEXT_BIT;
		vertices.push_back({ pos, color, SnakeCell::DIR_HAS_PREV_BIT });
	}

	void AddFood(Vec<2, int> pos) {
		vertices.push_back({ pos, Vec<3, float>{0,0,1}, 0 });
	}

	void Flush();

};


