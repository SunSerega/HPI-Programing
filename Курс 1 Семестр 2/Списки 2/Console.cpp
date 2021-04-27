#include "Console.h"

#include <Windows.h>
#include <string>



HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void Console::Clear(wchar_t fill)
{
    COORD tl = { 0,0 };

    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(console, &s);

    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);

    SetConsoleCursorPosition(console, tl);
}

void Console::SetTitle(std::wstring title)
{
    SetConsoleTitle(title.c_str());
}

std::wstring Console::ReadLine() {
    std::wstring res;
    std::getline(std::wcin, res);
    return res;
}

void Console::IgnoreLine()
{
    std::wcin.ignore((std::numeric_limits<std::streamsize>::max)(), L'\n');
}


