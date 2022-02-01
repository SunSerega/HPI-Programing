#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class StringWrap {
	string s;

public:
	StringWrap(string s) : s{ s } {}

	friend ostream& operator<<(ostream& otp, const StringWrap& w) {
		return otp << w.s;
	}

	friend ofstream& operator<<(ofstream& otp, const StringWrap& w) {
		for (auto ch : w.s) {
			if (isdigit(ch)) continue;
			if (isupper(ch)) continue;
			otp << ch;
		}
		return otp;
	}

};

int main()
{
	StringWrap w{ "abc ABCd 123e" };
	cout << w;
	ofstream{ "otp.txt" } << w;
}