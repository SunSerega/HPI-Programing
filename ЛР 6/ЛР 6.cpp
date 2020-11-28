#include <iostream>
#include <io.h>
#include <fcntl.h>

const double e = 0.000001;

/// <summary>
/// Вычисляет элемент ряда в номер n (считая с 0)
/// </summary>
double item_at(int n)
{
    return
        ((n % 2) ? -1 : 1) *
        ((double)n + 1) /
        pow(2, n - 1);
}

int main()
{
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stdin ), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

    {
        auto sum = 0.0;
        for (auto i = 0; i < 10; ++i)
            sum += item_at(i);
        wprintf(L"Сумма первых 10 элементов равна %lf\n", sum);
    }

    {
        auto sum = 0.0;
        double last_item;
        auto i = 0;
        while (abs(last_item = item_at(i++)) > e)
            sum += last_item;
        wprintf(L"Сумма первых %d элементов, каждый из которых >%e, равна %lf\n", i, e, sum);
    }

}