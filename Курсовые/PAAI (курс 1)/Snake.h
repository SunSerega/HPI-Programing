#pragma once

#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <memory>

#include "Vec.h"
#include "SnakeRenderer.h"



class Snake {
	Vec<2, int> head;
	std::deque<Vec<2, int>> body{};
	int dir, next_dir;
	double food = 3;
	int move_delay = 0;
	int max_move_delay;
	Vec<3, float> color;

	Snake* SplitAt(decltype(body)::iterator iter);
	void HeadLessReverse();
	static Snake* HitTestBody(Snake& sn1, Snake& sn2);

public:
	Snake(Vec<2, int> head, int dir, int move_delay, Vec<3, float> color)
		: head{ head }
		, dir{ dir }, next_dir{ dir }
		, max_move_delay{ move_delay }
		, color{ color }
	{}

	bool ChangeDir(int dir);

	std::vector<Vec<2, int>> TryEatAll(std::unordered_set<Vec<2, int>>& foods, double k);

	Snake* SelfHitTest();
	static std::array<Snake*, 2> HitTest(Snake& sn1, Snake& sn2);

	bool TryStarve();

	void Move(Vec<2, int> space_size);

	void Render(SnakeRenderer& r);

};

namespace std {
	template<>
	struct default_delete<Snake> {
		bool del;
		default_delete(bool del = true) : del(del) {}

		virtual void operator()(Snake* _Ptr) const noexcept {
			if (del) delete _Ptr;
		}

	};
}


