#include "MainWindow.h"

#include "glad/include/glad/gl.h"
#include "glad/include/glad/wgl.h"

#include <assert.h>
#include <string>
#include <fstream>

#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <unordered_set>

#include "Vec.h"
#include "OpenGLUtils.h"
#include "WindowUtils.h"

#include "Snake.h"
#include "SnakeRenderer.h"

using namespace std;





int window_w, window_h;
bool window_need_resize;
void MainWindow::Resize(int w, int h)
{
	window_w = w;
	window_h = h;
	window_need_resize = true;
}

queue<uint16_t> key_codes{};
mutex key_codes_mutex{};

void MainWindow::KeyDown(uint16_t key_code)
{
	switch (key_code)
	{
	case VK_ESCAPE:
		Halt(0);
		break;
	default:
		const lock_guard<mutex> lock{ key_codes_mutex };
		key_codes.push(key_code);
	}
}



void MainWindow::MainVisualLoop(std::function<void()> end_frame)
{

#pragma region Init
	
	//ToDo non-square sizes
	// - Only broken in window resizing
	auto field_size = Vec<2, int>{ 50, 50 };
	SnakeRenderer snake_renderer{ field_size };

	auto player = Snake{ {field_size[0] / 2, field_size[1] / 2}, 0, 10, {0,1,0} };

	auto all_snakes = vector<std::unique_ptr<Snake>>{};
	all_snakes.push_back(std::unique_ptr<Snake>{ &player, default_delete<Snake>(false) } );

	auto foods = unordered_set<Vec<2, int>>{};
	auto max_food = 100;

	/**/
	for (int i = 0; i < field_size[0]; ++i)
		if (rand() % 5 == 0)
			all_snakes.push_back(std::unique_ptr<Snake>{new Snake{
				Vec<2, int>{i, 0},
				0,
				15,
				Vec<3, float>{1,0,0}
			}});
	/**/

	/**/
	for (int i = 0; i < field_size[0]; ++i)
		if (rand() % 5 == 0)
			all_snakes.push_back(std::unique_ptr<Snake>{new Snake{
				Vec<2, int>{i, 0},
				0,
				12,
				Vec<3, float>{1,1,0}
			}});
	/**/

#pragma endregion

	while (running) {

#pragma region Physics

		//ToDo atomics
		//ToDo blinking
		if (window_need_resize) {
			window_need_resize = false;
			int w = min(window_w, window_h);
			glViewport((window_w - w) / 2 - w, (window_h - w) / 2 - w, w * 2, w * 2);
			snake_renderer.SetFieldPos({ 0, 0 }, { 1, 1 }, 1. / field_size[0]);
		}

		if (!key_codes.empty()) {
			const lock_guard<mutex> lock{ key_codes_mutex };
			auto k = key_codes.front();
			switch (k)
			{
			case VK_UP:
				player.ChangeDir(0);
				break;
			case VK_RIGHT:
				player.ChangeDir(1);
				break;
			case VK_DOWN:
				player.ChangeDir(2);
				break;
			case VK_LEFT:
				player.ChangeDir(3);
				break;
			}
			key_codes.pop();
		}

		if (foods.size() < max_food)
			foods.insert({ rand() % field_size[0], rand() % field_size[1] });
	
		//ToDo AI
		for (size_t i = 0; i < all_snakes.size(); ++i)
			if (all_snakes[i].get() != &player)
				all_snakes[i]->ChangeDir(rand() % 4);

		for (auto& sn : all_snakes)
			sn->Move(field_size);

		for (auto& sn : all_snakes)
			sn->TryEatAll(foods, 1);

		{
			auto new_snakes = vector<Snake*>{};

			for (int i = (int)all_snakes.size() - 1; i >= 0; --i)
				if (auto res = all_snakes[i]->SelfHitTest())
					new_snakes.push_back(res);

			for (size_t i1 = 0; i1+1 < all_snakes.size(); ++i1)
				for (size_t i2 = i1+1; i2 < all_snakes.size(); ++i2) {
					auto res = Snake::HitTest(*all_snakes[i1], *all_snakes[i2]);
					if (res[0]) new_snakes.push_back(res[0]);
					if (res[1]) new_snakes.push_back(res[1]);
				}

			for (int i = (int)all_snakes.size() - 1; i >= 0; --i)
				if (all_snakes[i]->TryStarve())
					all_snakes.erase(all_snakes.begin() + i);
			for (auto sn : new_snakes)
				if (!sn->TryStarve())
					all_snakes.push_back(unique_ptr<Snake>{ sn });

		}

#pragma endregion

#pragma region Render
		glClear(GL_COLOR_BUFFER_BIT);

		for (Vec<2, int> food : foods)
			snake_renderer.AddFood(food);

		for (auto& sn : all_snakes)
			sn->Render(snake_renderer);

		snake_renderer.Flush();

		glFinish();
		ThrowIfErr();
		end_frame();
#pragma endregion

	}
}


