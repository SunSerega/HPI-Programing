#include "MainWindow.h"

#include <windows.h>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <string>

#include <wingdi.h>
#include "glad/include/glad/gl.h"
#include "glad/include/glad/wgl.h"
/**
#include <gl/GL.h>
#include <gl/GLU.h>
/**/

bool MainWindow::running = true;

HINSTANCE opengl_dll;
void* LoadOpenGLFunction(char* name) {
	void* res;

	if (res = wglGetProcAddress(name)) return res;
	if (res = GetProcAddress(opengl_dll, name)) return res;

	throw 0;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	try
	{
		opengl_dll = LoadLibrary(L"opengl32.dll");
		if (!opengl_dll) throw 0;

		auto MainWindowClassName = L"PAAI Main window class name";

		WNDCLASSEX wcex; {
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = 0;// CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT {
				switch (message)
				{
				case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
				case WM_SIZE:
					MainWindow::Resize((lParam >> 0) & 0xFFFF, (lParam >> 16) & 0xFFFF);
					return 1;
				case WM_KEYDOWN:
					MainWindow::KeyDown(wParam); //ToDo WPARAM!=uint16
					return 0;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
			};
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground = nullptr;// (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = MainWindowClassName;
			wcex.hIconSm = nullptr;// LoadIcon(wcex.hInstance, IDI_APPLICATION);
		}

		if (!RegisterClassEx(&wcex)) {
			return -1;
		}

		auto window = CreateWindow(
			MainWindowClassName,
			L"PAAI",
			WS_MAXIMIZE | WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			0, 0,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);
		if (!window) {
			return -2;
		}

		ShowWindow(window, SW_MAXIMIZE);
		UpdateWindow(window);

		auto redraw_thread = std::thread{
			[window]() {
				//try
				{
					auto free_time = 0.0; // in frames
					auto last_t = std::chrono::system_clock::now();
					auto dc = GetDC(window);

					auto end_frame = [dc, &last_t, &free_time]() {
						SwapBuffers(dc);
						constexpr auto target_frames = 60;
						constexpr auto min_overhead = -3;

						using second_duration = std::chrono::duration<double>;
						using frame_duration = std::chrono::duration<double, std::ratio<1, target_frames>>;

						auto curr_t = std::chrono::system_clock::now();
						auto temp_free_time = free_time + 1 - frame_duration{ curr_t - last_t }.count();
						if (temp_free_time > 0)
						{
							std::this_thread::sleep_for(frame_duration{ -temp_free_time });
							temp_free_time = free_time + 1 - frame_duration{ curr_t - last_t }.count();
						}
						else if (temp_free_time < min_overhead)
							temp_free_time = min_overhead;

						free_time = temp_free_time;
						last_t = curr_t;
					};

					PIXELFORMATDESCRIPTOR pfd{}; {
						pfd.nSize = sizeof(pfd);
						pfd.nVersion = 1;
						pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
						pfd.cColorBits = 24;
						pfd.cDepthBits = 16;
					}
					if (!SetPixelFormat(dc, ChoosePixelFormat(dc, &pfd), &pfd))
						throw std::invalid_argument("failed to SetPixelFormat");

					auto context = wglCreateContext(dc);
					if (!wglMakeCurrent(dc, context))
						throw std::invalid_argument("failed to wglMakeCurrent");

					if (!gladLoadGL((GLADloadfunc)&LoadOpenGLFunction)) throw 1;
					if (!gladLoadWGL(dc, (GLADloadfunc)&LoadOpenGLFunction)) throw 2;

					MainWindow::MainVisualLoop(end_frame);
				}
				/**
				catch (char* err) {
					MessageBoxA(window, err, "Text error", 0);
				}
				catch (int err) {
					MessageBoxA(window, std::to_string(err).c_str(), "Code error", 0);
				}
				/**/
			}
		};

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		MainWindow::running = false;
		redraw_thread.join();

		return (int)msg.wParam;
	}
	catch (int err)
	{
		return err;
	}
}


