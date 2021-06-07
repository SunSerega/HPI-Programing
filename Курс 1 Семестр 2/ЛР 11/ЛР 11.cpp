


#include <iostream>
using namespace std;

#pragma region 11A

double RectArea(double w, double h = -1) {
	return w * (h == -1 ? w : h);
}

#pragma endregion

#pragma region 11B

template <class ... Ts>
void SwapMinMax(Ts& ... val) {
	constexpr auto size = sizeof...(val);
	common_type_t<Ts...>* a[size] = { &val... };
	auto ind_min = 0;
	auto ind_max = 0;
	for (int i = 1; i < size; ++i) {
		if (*a[i] > *a[ind_max]) ind_max = i; else
			if (*a[i] < *a[ind_min]) ind_min = i;
	}
	swap(*a[ind_min], *a[ind_max]);
}

template <class ... Ts>
void SwapMinMax(Ts* ... val) {
	constexpr auto size = sizeof...(val);
	common_type_t<Ts...>* a[size] = { val... };
	auto ind_min = 0;
	auto ind_max = 0;
	for (int i = 1; i < size; ++i) {
		if (*a[i] > *a[ind_max]) ind_max = i; else
			if (*a[i] < *a[ind_min]) ind_min = i;
	}
	swap(*a[ind_min], *a[ind_max]);
}

#pragma endregion

#pragma region 11C

class Sum {
private:
	double x, y;
	double s = 0;

public:
	Sum(double x, double y)
		: x(x)
		, y(y)
	{};

	void Init(double x, double y) {
		this->x = x;
		this->y = y;
	}

	void operator() () {
		s = x + y;
	}

	void Print() {
		cout << "x = " << x << "; y = " << y << endl;
		cout << "s = " << s << endl;
	}

};

#pragma endregion

#pragma region 11D

class Timer {
private:
	unsigned long seconds;

public:
	//Timer(unsigned long seconds) {
	//	this->seconds = seconds;
	//}

	Timer(unsigned long seconds, unsigned long minutes = 0) {
		this->seconds = seconds + minutes * 60;
	}

	Timer(const Timer& t) {
		this->seconds = t.seconds;
	}

	void Print() {
		cout << "Seconds: " << seconds << endl;
	}

};

#pragma endregion

int main()
{

	cout << "11A:" << endl; {
		cout << "Enter rect size (2 numbers): ";
		double w, h;
		cin >> w >> h;
		cout << "Area = " << RectArea(w, h) << endl;

		cout << "Enter square size (1 number): ";
		cin >> w;
		cout << "Area = " << RectArea(w) << endl;
	}

	cout << "11B:" << endl; {
		cout << "Enter 3 numbers: ";
		double a, b, c;
		cin >> a >> b >> c;
		SwapMinMax(a, b, c);
		cout << "Min and Max swapped: " << a << ' ' << b << ' ' << c << endl;

		cout << "Enter 3 numbers (again): ";
		cin >> a >> b >> c;
		SwapMinMax(&a, &b, &c);
		cout << "Min and Max swapped: " << a << ' ' << b << ' ' << c << endl;
	}

	cout << "11C:" << endl; {
		cout << "Enter x and y: ";
		double x, y;
		cin >> x >> y;
		auto s = Sum{ x, y };
		s();
		s.Print();
	}

	cout << "11D:" << endl; {
		cout << "Enter seconds and minutes: ";
		unsigned long seconds, minutes;
		cin >> seconds >> minutes;
		auto t = Timer{ seconds, minutes };
		t.Print();
	}

}


