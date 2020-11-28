#include <iostream>
#include <io.h>
#include <fcntl.h>

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin ), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    wprintf(L"Введите [x] и [y] через пробел: ");
    double x, y;
    wscanf_s(L"%lf %lf", &x, &y);

    if (x * x + y * y < 1 * 1 && (x > 0 || y > 0))
        wprintf(L"Точка (x=%lf; y=%lf) попадает в область", x, y); else
        wprintf(L"Точка (x=%lf; y=%lf) НЕ попадает в область", x, y);

}