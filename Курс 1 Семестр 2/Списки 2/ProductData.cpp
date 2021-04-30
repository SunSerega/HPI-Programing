#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "ProductData.h"
#include "LList.h"

using namespace std;

template <class T>
auto int_len(T x)
{
	static_assert(std::is_integral<T>(), "int_len is for int");
	auto res = 0;
	do {
		res += 1;
		x /= 10;
	} while (x);
	return res;
};

#define PDColCount 4
#define PDDisplayW(p) {(int)(p).name.length(), (int)(p).producer.length(), int_len((p).part_count), int_len((p).cost_in_dollars)}

void WriteTableHeader(wostream& os, const wstring& title, int len) {
	len -= (int)title.length() + 2;
	const auto len2 = len / 2;
	const auto len1 = len - len2;
	os << wstring(len1, L'═') << L' ' << title << L' ' << wstring(len2, L'═');
}

wostream& operator<<(wostream& os, const List<ProductData>& data)
{
	const wstring headers[]{ L"Name", L"Producer", L"Part count", L"Cost ($)" };

	int w[PDColCount]{};
	for (int i = 0; i < PDColCount; ++i)
		w[i] = (int)headers[i].length()+2;

	for (auto& p : data)
	{
		int nw[] = PDDisplayW(p);
		for (int i = 0; i < PDColCount; ++i)
			w[i] = max(w[i], nw[i]);
	}

	for (int i = 0; i < PDColCount; ++i) {
		os << (i ? L'╦' : L'╔');
		WriteTableHeader(os, headers[i], w[i] + 2);

	}
	os << L'╗' << endl;

	for (auto& pd : data)
	{
		os << L"║ "
			<< setw(w[0]) << left << pd.name << L" ║ "
			<< setw(w[1]) << left << pd.producer << L" ║ "
			<< setw(w[2]) << right << pd.part_count << L" ║ "
			<< setw(w[3]) << right << pd.cost_in_dollars << L" ║"
		<< endl;
	}

	for (int i = 0; i < PDColCount; ++i) {
		os << (i ? L'╩' : L'╚') << wstring(w[i]+2, L'═');
	}
	os << L'╝' << endl;

	return os;
}

wistream& operator>>(wistream& is, ProductData& data)
{

	wcout << L"Enter product name: ";
	getline(wcin, data.name);
	if (!data.name.length()) {
		is.setstate(ios::failbit);
		return is;
	}

	wcout << L"Enter product procucer: ";
	getline(is, data.producer);

	wcout << L"Enter part count: ";
	is >> data.part_count;
	is.ignore(numeric_limits<streamsize>::max(), L'\n');

	wcout << L"Enter cost ($): ";
	is >> data.cost_in_dollars;
	is.ignore(numeric_limits<streamsize>::max(), L'\n');

	return is;
}
