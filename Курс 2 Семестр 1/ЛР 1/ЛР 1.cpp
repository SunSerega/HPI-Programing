#include <iostream>
#include <numbers>
#include <vector>
using namespace std;

#pragma region A

class Point {
	double x, y;

public:
	Point()
		: x{}, y{}
	{};

	Point(double x, double y)
		: x{ x }, y{ y }
	{};

	Point(const Point& p)
		: x{ p.x }, y{ p.y }
	{};

	void set(double x, double y) {
		this->x = x;
		this->y = y;
	}

	void sum_mlt(double& sum, double& mlt) {
		sum = x + y;
		mlt = x * y;
	}

	friend istream& operator>>(istream& inp, Point& p) {
		return inp >> p.x >> p.y;
	}
	friend ostream& operator<<(ostream& otp, const Point& p) {
		return otp << "Point[" << p.x << ", " << p.y << "]";
	}

};

void A() {

	Point p;
	cout << "Enter point coords: ";
	cin >> p;
	cout << "original=" << p << " copy=" << Point{p} << endl;

	double sum, mlt;
	p.sum_mlt(sum, mlt);
	cout << "sum=" << sum << " mlt=" << mlt << endl;

}

#pragma endregion

#pragma region B

class Figure {
	double w, h;

public:
	Figure(double w, double h = NAN)
		: w{ w }, h{ h }
	{}

	double operator()() {
		return isnan(h) ?
			std::numbers::pi_v<double>*w*w/4 :
			w * h / 2;
	}

};

void B() {

	cout << "Enter w,h: ";
	double w, h;
	cin >> w >> h;

	cout << "Area of the circle with diameter " << w << " = " << Figure{ w }() << endl;
	cout << "Area of the triangle with base " << w << " and height " << h << " = " << Figure{ w, h }() << endl;

}

#pragma endregion

#pragma region C

class Complex {
	double r, i;

public:
	Complex()
		: r{}, i{}
	{}

	Complex(double r, double i)
		: r{ r }, i{ i }
	{}

	friend Complex operator+(const Complex& c1, const Complex& c2) {
		return { c1.r + c2.r, c1.i + c2.i };
	}

	friend istream& operator>>(istream& inp, Complex& c) {
		return inp >> c.r >> c.i;
	}
	friend ostream& operator<<(ostream& otp, const Complex& c) {
		return otp << c.r << "+i*" << c.i;
	}

};

void C() {

	cout << "Enter real and imaginary parts of complex c: ";
	Complex c;
	cin >> c;
	cout << "Reconstructed c: " << c << endl;

	cout << "c+c = " << c + c << endl;

}

#pragma endregion

#pragma region D

class Dog {
	string name;
	double weight, age;

public:
	Dog(string name, double weight)
		: name{ name }
		, weight{ weight }, age{0}
	{}
	Dog(string name, double weight, double age)
		: name{ name }
		, weight{ weight }, age{ age }
	{}

	auto get_name() {
		return name;
	}
	auto get_weight() {
		return weight;
	}
	auto get_age() {
		return age;
	}

	void update_name(string name) {
		this->name = name;
	}
	void update_info(double weight, double age) {
		this->weight = weight;
		this->age = age;
	}

};
class Master {
	string name;
	static int last_id;
	const int id = ++last_id;
	vector<Dog> dogs{};

public:
	Master(string name)
		: name{ name }
	{}

	void update_name(string name) {
		this->name = name;
	}

	template <class T = Dog>
	void add_dog(T&& dog) {
		dogs.push_back(forward<T>(dog));
	}

	auto get_name() {
		return name;
	}

	auto get_id() {
		return id;
	}

	auto begin() {
		return dogs.begin();
	}
	auto end() {
		return dogs.end();
	}

};
int Master::last_id = 0;

void D() {

	Master m{ "Mike" };

	m.add_dog({ "dog1", 12 });
	m.add_dog({ "dog2", 15, 3 });

	cout << "Master " << m.get_name() << "#" << m.get_id() << " has dogs:" << endl;
	for (auto& d : m) {
		cout << "\tdog \"" << d.get_name() << "\""
			<< " weighs " << d.get_weight()
			<< " at the age of " << d.get_age()
		<< endl;
	}

}

#pragma endregion

int main()
{
	A();
	B();
	C();
	D();
}