#include <iostream>
#include <string>
#include <map>
#include <iomanip>
using namespace std;

string hungry_get_line(istream& inp) {
	string res;
	char ch;
	while (inp.get(ch) && !isspace(ch))
		res += ch;
	return res;
}

class PersonName {
	string name;

public:
	PersonName() : name{} {}
	PersonName(string name) : name{ name } {}

	bool is_empty() const { return name.empty(); }

	bool starts_with(const string& s) const { return name.starts_with(s); }

	friend istream& operator>>(istream& inp, PersonName& pn) {
		pn = hungry_get_line(inp);
		return inp;
	}

	friend ostream& operator<<(ostream& otp, const PersonName& pn) {
		return otp << pn.name;
	}

	friend strong_ordering operator<=>(const PersonName&, const PersonName&) = default;

};

class PhoneNumber {
	int32_t digits;

public:
	PhoneNumber() : digits{} {};
	PhoneNumber(int32_t digits)
		: digits{ digits }
	{}
	PhoneNumber(string number) {
		digits = atoi(number.c_str() + 3 * number.starts_with("+38"));
	}

	friend istream& operator>>(istream& inp, PhoneNumber& n) {
		n = hungry_get_line(inp);
		return inp;
	}

	friend ostream& operator<<(ostream& otp, const PhoneNumber n) {
		return otp << "+380" << setw(9) << setfill('0') << n.digits;
	}
	
};

template <class T1, class T2>
ostream& operator<<(ostream& otp, const pair<T1, T2>& p) {
	return otp << p.first << ' ' << p.second;
}

int main()
{
	map<PersonName, PhoneNumber> m{};
	while (true) {

		cout << "Name: ";
		PersonName pn;
		cin >> pn;
		if (pn.is_empty()) break;

		cout << "Number: ";
		PhoneNumber n;
		cin >> n;

		decltype(m)::value_type p{ pn, n };
		cout << p << endl;
		m.emplace(move(p));
	}

	for (auto& p : m)
		cout << p << endl;

	{
		cout << "Enter first letters: ";
		erase_if(m,
			[name_start = hungry_get_line(cin)](decltype(m)::value_type p) {
				auto res = p.first.starts_with(name_start);
				if (res) cout << p << endl;
				return res;
			}
		);
	}

	cout << string(50, '=') << endl;
	for (auto& p : m)
		cout << p << endl;

	while (true) {
		cout << "Name: ";
		PersonName pn;
		cin >> pn;

		auto iter = m.find(pn);
		if (iter == m.end())
			cout << "Name not found" << endl; else
			cout << iter->second << endl;

	}

}