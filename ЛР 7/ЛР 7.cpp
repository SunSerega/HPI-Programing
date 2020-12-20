#include <iostream>
#include <io.h>
#include <fcntl.h>

// Для получения размера окна консоли
#include <windows.h>
#define CONSOLE_HANDLE GetStdHandle(STD_OUTPUT_HANDLE)

using namespace std;

#define XMin 0.0
#define XMax 18.0

double f(double x)
{
    auto pos = fmod(x, 4);
    return pos < 2 ?
        2 - pos :
        2 - sqrt(4 * pos - pos * pos);
}

BOOL draw_line(double x1, double y1, double x2, double y2)
{
    auto point_c = (int)ceil( max(abs(x1-x2), abs(y1-y2)) + 1);
    for (auto i = 0; i < point_c; ++i)
    {
        if (!SetConsoleCursorPosition(CONSOLE_HANDLE, COORD{
            (SHORT)round(x1 + (x2 - x1) * (i / ((double)point_c - 1))),
            (SHORT)round(y1 + (y2 - y1) * (i / ((double)point_c - 1)))
        })) return false;
        wcout.put(L'*');
    }
    return true;
}

int GetConsoleWindowHeigth()
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(CONSOLE_HANDLE, &info))
        return -1;
    return info.srWindow.Bottom - info.srWindow.Top + 1;
}

int main()
{
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stdin ), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

    auto lines_c = GetConsoleWindowHeigth();
    if (lines_c == -1) return -1;

#if _DEBUG
    lines_c -= 4; // В конце выводит "press Enter to exit" и т.п.
#else _DEBUG
    lines_c -= 1;
#endif _DEBUG

    const auto graph_offset = 25;
    auto graph_scale = ((double)lines_c - 1) / (XMax - XMin);
    const auto graph_x_scale = 2.0;

    double prev_x{}, prev_y{};
    for (auto i = 0; i < lines_c; ++i)
    {
        double graph_x = XMin + (XMax - XMin) * i / (lines_c - (double)1);
        double graph_y = f(graph_x);

        if (!SetConsoleCursorPosition(CONSOLE_HANDLE, COORD{ 0, (SHORT)i })) return -1;
        wprintf(L"x = %5.2lf; y = %4.2lf |", graph_x, graph_y);

        double x = graph_y * graph_scale * graph_x_scale + graph_offset;
        double y = graph_x * graph_scale;

        if (i != 0)
            if (!draw_line(prev_x, prev_y, x, y)) return -1;

        prev_x = x;
        prev_y = y;
    }

}