#include "DB.h"
#include <iostream>
using namespace std;

int main()
{
	auto db = new DB{};
	while (true) {
		cout << *db;
		cout << "Choose what to do:" << endl;
		cout << "0: exit" << endl;
		cout << "1: Add a new row" << endl;
		cout << "2: Remove a row at" << endl;
		cout << "3: Sort all by name" << endl;
		int choise;
		cin >> choise;
		if (!choise) break;
		switch (choise)
		{
		case 1:
			db->AddRow(new CultureInfo{ cin });
			break;
		case 2:
			cout << "Enter row index" << endl;
			int ind;
			cin >> ind;
			db->DelRow(ind);
			break;
		case 3:
			db->SortByName();
			break;
		default:
			cout << "Wrong choise id" << endl;
		}
		cout << endl;
	}
	delete db;
}