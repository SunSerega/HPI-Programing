#include "CultureInfo.h"
#include <iostream>
using namespace std;

istream& operator>>(istream& stream, CultureInfo& info) {
	using CultureType = CultureInfo::CultureType;
	cout << "Enter name: ";
	stream >> info.name;
	cout << "Enter culture type:" << endl;
	cout << "\t1 = cereal" << endl;
	cout << "\t2 = bean" << endl;
	int i;
	stream >> i;
	info.type = (CultureType)(i - 1);
	cout << "Enter area in use: ";
	stream >> info.area_in_use;
	cout << "Enter efficiency: ";
	stream >> info.efficiency;
	return stream;
}

ostream& operator<<(ostream& stream, const CultureInfo& info) {
	stream << "name: " << info.name << "; ";
	static const char* type_name_table[] = { "cereal", "bean" };
	stream << "type: " << type_name_table[(int)info.type] << "; ";
	stream << "area_in_use: " << info.area_in_use << "; ";
	stream << "efficiency: " << info.efficiency << ";";
	return stream;
}
