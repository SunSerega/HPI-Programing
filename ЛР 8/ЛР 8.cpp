#include <iostream>
#include <io.h>
#include <fcntl.h>

using namespace std;

const int arr_len = 200;
const int chain_len = 10;

inline int Random(int x, int y)
{
    return rand() % (y - x + 1) + x;
}

int main()
{
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stdin ), _O_U16TEXT) == -1) return -1;
    if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

    int arr[arr_len];
    for (auto i = 0; i < arr_len; ++i)
        arr[i] = Random(0, 100); // Можно и rand() % 101, но так код более читабельный //ToDo в отчёт

    for (auto i = 0; i < arr_len; ++i)
        wcout << arr[i] << ' ';
    wcout << endl;

    int chain[chain_len];
    copy(arr, arr+10, chain);

    int sum = 0;
    for (auto i = 0; i < chain_len; ++i)
        sum += chain[i];

    int max_sum = sum;
    int max_sum_ind = chain_len - 1;
    for (auto i = chain_len; i < arr_len; ++i)
    {
        auto chain_ind = i % chain_len;
        sum -= chain[chain_ind];
        auto val = arr[i];
        sum += val;
        if (sum > max_sum)
        {
            max_sum = sum;
            max_sum_ind = i;
        }
        chain[chain_ind] = val;
    }

    wcout << L"Участок с максимальной суммой:" << endl;

    for (auto i = max_sum_ind - (chain_len-1); i < max_sum_ind; ++i)
        wcout << arr[i] << ' ';
    wcout << endl;

    wcout << L"Имеет сумму: " << max_sum << endl;
}