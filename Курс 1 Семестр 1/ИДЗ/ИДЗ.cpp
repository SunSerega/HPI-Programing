#include <iostream>
#include <io.h>
#include <fcntl.h>

using namespace std;

int main()
{
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stdin ), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

    int n;
    wcout << L"Введите число: ";
    wcin >> n;

    auto sum1 = 0;
    auto sum2 = 0;
    while (n)
    {
        auto digit = n % 10;

        if (n % 2)
            // Нечётная цифра
            sum2 += digit * digit * digit; else
            // Чётная цифра
            sum1 += digit * digit;

        n /= 10;
    }

    wcout << L"Сумма квадратов чётных цифр: "   << sum1 << endl;
    wcout << L"Сумма кубов нечётных цифр: "     << sum2 << endl;
}