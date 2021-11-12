#include <iostream>
#include <string>
using namespace std;

enum class CultureType { зерновые, бобовые };

class base {
protected:
	string name{};
	CultureType type{};
	int area_in_use{};
	float efficiency{};

public:
	base() {
		cout << "%constructor%" << endl;
		cout << "Enter name: ";
		cin >> this->name;
		cout << "Enter culture type:" << endl;
		cout << "\t1 = cereal" << endl;
		cout << "\t2 = bean" << endl;
		int i;
		cin >> i;
		switch (i)
		{
		case 1:
			this->type = CultureType::зерновые;
			break;
		case 2:
			this->type = CultureType::бобовые;
			break;
		default:
			throw i;
		}
		cout << "Enter area in use: ";
		cin >> this->area_in_use;
		cout << "Enter efficiency: ";
		cin >> this->efficiency;
	}

	base(string name, CultureType type, int area_in_use, float efficiency)
		: name{ name }
		, type{ type }
		, area_in_use{ area_in_use }
		, efficiency{ efficiency }
	{}

	friend ostream& operator<<(ostream& out, const base& a) {
		out << "%contents%" << endl;
		out << "name: " << a.name << endl;
		static const char* type_name_table[] = { "cereal", "bean" };
		out << "type: " << type_name_table[(int)a.type] << endl;
		out << "area_in_use: " << a.area_in_use << endl;
		out << "efficiency: " << a.efficiency << endl;
		return out;
	}

};

class A : public base {

public:
	A() : base{} {};
	A(string name, CultureType type, int area_in_use, float efficiency)
		: base{ name, type, area_in_use, efficiency }
	{};

	bool operator==(const A& other) const {
		return
			name == other.name &&
			type == other.type &&
			area_in_use == other.area_in_use &&
			efficiency == other.efficiency;
	}

	void operator=(const A& other) {
		name = other.name;
		type = other.type;
		area_in_use = other.area_in_use;
		efficiency = other.efficiency;
	}

	A operator+(const A& other) const {
		if (type != other.type) throw 0;
		return {
			name + '+' + other.name,
			type,
			area_in_use + other.area_in_use,
			(efficiency * area_in_use + other.efficiency * other.area_in_use) / (area_in_use + other.area_in_use),
		};
	}

};

class B : public base {

public:
	B() : base{} {};
	B(string name, CultureType type, int area_in_use, float efficiency)
		: base{ name, type, area_in_use, efficiency }
	{};

	friend bool operator==(const B& o1, const B& o2) {
		return
			o1.name == o2.name &&
			o1.type == o2.type &&
			o1.area_in_use == o2.area_in_use &&
			o1.efficiency == o2.efficiency;
	}

	friend B operator+(const B& o1, const B& o2) {
		if (o1.type != o2.type) throw 0;
		B res;
		return {
			o1.name + '+' + o2.name,
			o1.type,
			o1.area_in_use + o2.area_in_use,
			(o1.efficiency * o1.area_in_use + o2.efficiency * o2.area_in_use) / (o1.area_in_use + o2.area_in_use),
		};
	}

	void operator()(string name, CultureType type, int area_in_use, float efficiency) {
		this->name = name;
		this->type = type;
		this->area_in_use = area_in_use;
		this->efficiency = efficiency;
	}

};

int main()
{
	{
		cout << "a1: ";
		A a1{};
		auto a2 = a1;
		cout << "a1 == a2(copy): " << (a1 == a2 ? "true" : "false") << endl;
		cout << "a2: ";
		a2 = A{};
		cout << "a1 == a2: " << (a1 == a2 ? "true" : "false") << endl;
		cout << "a1 + a2: " << a1 + a2;
	}

	cout << endl << endl << endl;

	{
		cout << "b1: ";
		B b1{};
		cout << "b2: ";
		B b2{};
		cout << "b1 == b2: " << (b1 == b2 ? "true" : "false") << endl;
		b2("name2", CultureType::зерновые, 3, 4.5);
		cout << "b1 + b2: " << b1 + b2;
	}

}