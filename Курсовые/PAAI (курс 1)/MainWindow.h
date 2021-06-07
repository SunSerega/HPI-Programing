#pragma once

#include <functional>



class MainWindow
{
public:
	MainWindow() = delete;

	static bool running;
	static void MainVisualLoop(std::function<void()> end_frame);

	static void Resize(int w, int h);

	static void KeyDown(uint16_t key_code);

};


