#include "Snake.h"

using namespace std;



bool Snake::ChangeDir(int dir) {
	if (((this->dir - dir) & 0b11) == 2) return false;
	next_dir = dir;
	return true;
}

vector<Vec<2, int>> Snake::TryEatAll(unordered_set<Vec<2, int>>& foods, double k) {
	vector<Vec<2, int>> eaten{};
	for (Vec<2, int> b : body)
		if (foods.erase(b)) eaten.push_back(b);
	this->food += eaten.size() * k;
	return eaten;
}

int CalcDir(Vec<2, int> dir) {
	return
		(dir[1] == +1) || (dir[1] < -1) ? 0 :
		(dir[0] == +1) || (dir[0] < -1) ? 1 :
		(dir[1] == -1) || (dir[1] > +1) ? 2 :
		(dir[0] == -1) || (dir[0] > +1) ? 3 :
	throw dir;
}

Snake* Snake::SplitAt(decltype(body)::iterator iter)
{

	auto res = new Snake(
		deque<Vec<2, int>>(body.rbegin(), decltype(body.rbegin()){ iter + 1 }),
		CalcDir(body.end()[-1]-body.end()[-2]),
		max_move_delay,
		color
	);
	res->food = 0;
	res->move_delay = max_move_delay-1;

	body.erase(iter, body.end());
	return res;
}
void Snake::HeadLessReverse() {
	food = 0;

	body.pop_front();
	if (body.size() < 2) return;
	reverse(body.begin(), body.end());

	dir = CalcDir(body[0]-body[1]);
	next_dir = dir;
}

Snake* Snake::SelfHitTest() {
	for (auto iter = body.begin()+1; iter != body.end(); ++iter)
		if (body[0] == *iter) {
			food += 1;
			return SplitAt(iter);
		}
	return nullptr;
}
Snake* Snake::HitTestBody(Snake& sn1, Snake& sn2) {
	for (auto iter = sn1.body.begin()+1; iter != sn1.body.end(); ++iter)
		if (sn2.body[0] == *iter) {
			sn2.food += 0.5;
			return sn1.SplitAt(iter);
		}
	return nullptr;
}

array<Snake*, 2> Snake::HitTest(Snake& sn1, Snake& sn2) {
	if (sn1.body.size() < 2) return {};
	if (sn2.body.size() < 2) return {};

	if (sn1.body[0] == sn2.body[0]) {
		sn1.HeadLessReverse();
		sn2.HeadLessReverse();
		return {};
	}

	auto nsn1 = HitTestBody(sn1, sn2);
	auto nsn2 = HitTestBody(sn2, sn1);

	return {nsn1, nsn2};
}

bool Snake::TryStarve()
{
	if (food <= -1) {
		body.pop_back();
		food += 1;
	}
	return body.size() < 2;
}

void Snake::Move(Vec<2, int> space_size) {
	if (move_delay) {
		--move_delay;
		return;
	}
	else
		move_delay = max_move_delay;
	dir = next_dir;

	food -= 0.01;
	if (food >= 1)
		food -= 1;
	else
		body.pop_back();

	static const Vec<2, int> dir_offset[4]{
		{ +0, +1 },
		{ +1, +0 },
		{ +0, -1 },
		{ -1, +0 },
	};
	auto head = body.front() + dir_offset[dir] + space_size;
	head[0] %= space_size[0];
	head[1] %= space_size[1];
	body.push_front(head);
}

void Snake::Render(SnakeRenderer& r)
{
	for (Vec<2, int> b : body)
		r.AddBody(b, this->color);
	r.EndSnake();
}
