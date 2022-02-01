#include <iostream>
using namespace std;

template <class T>
class Sum {
	T val;

public:
	// К-торы
	Sum() : val{} {};
	Sum(T val) : val{ val } {};

	friend istream& operator>>(istream& inp, Sum& x) {
		T val;
		inp >> val;
		x = { val };
		return inp;
	}

	// operator+
	friend Sum operator+(const Sum& s1, const Sum& s2) {
		return { s1.val + s2.val };
	}

	// Оператор преобразования Sum в T, для вывода внутреннего значения
	operator T() {
		return val;
	}

};

int main()
{

	{
		Sum<int> s1, s2;
		cin >> s1 >> s2;
		cout << s1 + s2 << endl;
	}

	{
		Sum<double> s1, s2;
		cin >> s1 >> s2;
		cout << s1 + s2 << endl;
	}

}