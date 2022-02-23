#include <iostream>
#include "DB.h"
#include <algorithm>
using namespace std;

void DB::AddRow(CultureInfo* info) {
	sorted = false;
	infos.push_back(info);
}

void DB::DelRow(int ind) {
	sorted = false;
	auto iter = infos.begin() + ind;
	delete* iter;
	infos.erase(iter);
}

void DB::SortByName() {
	if (sorted) return;
	sorted = true;
	sort(
		infos.begin(), infos.end(),
		[](auto p1, auto p2) {
			return *p1 < *p2;
		}
	);
}

ostream& operator<<(ostream& stream, const DB& db) {
	stream << "Database has " << db.infos.size() << " items:" << endl;
	for (auto info : db.infos)
		stream << '\t' << *info << endl;
	return stream;
}
