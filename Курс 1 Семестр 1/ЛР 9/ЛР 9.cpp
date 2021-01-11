#include <iostream>
#include <iomanip>
#include <io.h>
#include <fcntl.h>

using namespace std;

const int matr_w = 9;

int main()
{
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stdin ), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

    int mtr[matr_w][matr_w];

    int c = 1;

    // Заполнение части под главной диагональю (включительно)
    for (auto i1 = 0; i1 < matr_w; ++i1)
        for (auto i2 = 0; i2 <= i1; ++i2)
            mtr[matr_w - 1 - i2][i1 - i2] = c++;

    // Заполнение части над главной диагональю (НЕ_включительно)
    for (auto i1 = 1; i1 < matr_w; ++i1)
        for (auto i2 = 0; i2 < matr_w-i1; ++i2)
            mtr[matr_w-1-i1 - i2][matr_w-1 - i2] = c++;

    for (auto row = 0; row < matr_w; ++row)
    {
        for (auto col = 0; col < matr_w; ++col)
            wcout << setw(3) << mtr[row][col];
        wcout << endl;
    }

}