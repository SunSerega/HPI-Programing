#include <iostream>
#include <algorithm>
#include "CarInRepair.h"
using namespace std;

#define N 25
CarInRepair db[N];

int main()
{
	srand(0);

	for (int i = 0; i < N; ++i)
		CarInRepair::Generate(db[i]);

	sort(db, db + N);

	for (int i = 0; i < N; ++i)
		cout << db[i] << endl;

}