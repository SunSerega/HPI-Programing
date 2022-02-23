
#include <iostream>
#include <algorithm>
#include "CarInRepair.h"

#include <ranges>
using namespace std;

#define N 25

int main()
{
	srand(0x56702957);
	auto db = reinterpret_cast<CarInRepair*>(operator new[](N * sizeof(CarInRepair)));

	for (int i = 0; i < N; ++i)
		CarInRepair::Generate(db[i]);

	sort(db, db+N);
	for (int i = 0; i < N; ++i)
		cout << db[i] << endl;

	operator delete[](db);
}