#include <iostream>
#include <io.h>
#include <fcntl.h>

#include <math.h>

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin ), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    // Ограничения на a и b:
    // > a != 0
    // > b != 0
    double a = 12.5, b = 1.3;

    auto a_d2 = a / 2;

    double x, y;
    wprintf(L"Ограничения на x и y:\n");
    wprintf(L"> x != 0\n");
    wprintf(L"> y != 0\n");
    wprintf(L"> y/x > 0\n");
    wprintf(L"> %lf * x %% Pi != Pi/2\n", a_d2);
    wprintf(L"> %lf * x %% (2*Pi) ∈(принадлежит) [0; Pi]\n", a_d2);
    wprintf(L"Введите значения x и y: ");
    wscanf_s(L"%lf %lf", &x, &y);

    auto ax = a * x;

    // Умножение менее затратное чем деление, поэтому лучше вычислять как (a_d2 * x) чем как (ax / 2)
    auto ax_d2 = a_d2 * x;

    auto t1 = (log(y / x) + ax / y) / (b * b);
    auto t2 = x / a * tan(ax_d2) + 2 / (a * a) * log(sin(ax_d2));

    wprintf(L"t1 = %lf\n", t1);
    wprintf(L"t2 = %lf\n", t2);

}