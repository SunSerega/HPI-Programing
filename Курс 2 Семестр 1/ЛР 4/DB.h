#pragma once
#include "CultureInfo.h"
#include <vector>

class DB {
private:
	std::vector<CultureInfo*> infos{};
	bool sorted = false;

public:
	DB() {};

	void AddRow(CultureInfo* info);

	void DelRow(int ind);

	void SortByName();

	friend std::ostream& operator<<(std::ostream& stream, const DB& db);

};
