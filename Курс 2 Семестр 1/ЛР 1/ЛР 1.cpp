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

	friend ostream& operator<<(ostream& otp, const Point& p) {
		return otp << "Point[" << p.x << ", " << p.y << "]";
	}

};

void A() {

	cout << "Enter point coords: ";
	double x, y;
	cin >> x >> y;
	Point p{ x,y };
	cout << "Empty=" << Point{} << ' '
	     << "Reconstructed=" << p << ' '
	     << "Copy=" << Point{ p } << endl;

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

	void AddR(int r) {
		this->r += r;
	}
	void AddI(int i) {
		this->i += i;
	}

	friend Complex operator+(const Complex& c1, const Complex& c2) {
		return { c1.r + c2.r, c1.i + c2.i };
	}

	friend ostream& operator<<(ostream& otp, const Complex& c) {
		return otp << c.r << "+i*" << c.i;
	}

};

void C() {

	cout << "Enter real and imaginary parts of complex c: ";
	double r, i;
	cin >> r >> i;
	Complex c{r, i};
	cout << "Empty=" << Complex{} << "Reconstructed c: " << c << endl;

	cout << "c+c = " << c + c << endl;

}

#pragma endregion

#pragma region D

class Dog {
	string name;
	double weight, age;

public:
	Dog(string name, double weight, double age = 0)
		: name{ name }
		, weight{ weight }, age{ age }
	{}

	auto get_name() const {
		return name;
	}
	auto get_weight() const {
		return weight;
	}
	auto get_age() const {
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

	auto get_name() const {
		return name;
	}

	auto get_id() const {
		return id;
	}

	auto begin() const {
		return dogs.begin();
	}
	auto end() const {
		return dogs.end();
	}

};
int Master::last_id = 0;

ostream& operator<<(ostream& otp, const Master& m) {
	otp << "Master " << m.get_name() << "#" << m.get_id() << " has dogs:" << endl;
	for (auto& d : m) {
		cout << "\tdog \"" << d.get_name() << "\""
			<< " weighs " << d.get_weight()
			<< " at the age of " << d.get_age()
			<< endl;
	}
	return otp;
}

void D() {

	Master m1{ "m.First" };
	m1.add_dog({ "dog1", 12 });
	m1.add_dog({ "dog2", 15, 3 });
	cout << m1;

	Master m2{ "m.Second" };
	m2.add_dog({ "dog1", 13 });
	cout << m2;

}

#pragma endregion

int main()
{
	A();
	B();
	C();
	D();
}