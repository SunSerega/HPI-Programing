#pragma once

#include <functional>

#include "WindowUtils.h"



class MainWindow
{
public:
	MainWindow() = delete;

	static bool running;
	static void MainVisualLoop(std::function<void()> end_frame);

	static Event resize_ev;
	static void Resize(int w, int h);

	static void KeyDown(uint16_t key_code);

};


