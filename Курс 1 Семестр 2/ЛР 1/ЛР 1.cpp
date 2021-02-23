// ЛР 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;

void ReverseOtp() {
    int x;
    cin >> x;
    if (!x) return;
    ReverseOtp();
    cout << x << ' ';
}

int main()
{
    std::cout << "Enter sequence of integers, using 0 to mark end of sequence:\n";
    ReverseOtp();
}