#include <iostream>
using namespace std;

class Value {
	int v;

public:
	Value(int v)
		: v{ v }
	{}

};

class ValueDiv7 : Value {
	int mod7;

public:
	ValueDiv7(int v)
		: Value{ v }
		, mod7{ v % 7 }
	{}

	bool IsRight() { return mod7 == 3; }

};

class ValueDiv5 : ValueDiv7 {
	int mod5;

public:
	ValueDiv5(int v)
		: ValueDiv7{ v }
		, mod5{ v % 5 }
	{}

	bool IsRight() { return ValueDiv7::IsRight() && mod5 == 2; }

};

int main()
{
	cout << "First right values:" << endl;
	for (int i = 0; i < 200; ++i)
		if (ValueDiv5(i).IsRight())
			cout << i << endl;

	while (true) {
		cout << "Enter value: ";
		int x;
		cin >> x;
		cout << boolalpha << ValueDiv5(x).IsRight() << endl;
	}

}

