#include <iostream>
#include <string>
using namespace std;

class Dog {
	float weight, age;

public:
	Dog(float weight, float age)
		: weight{ weight }, age{ age }
	{
		cout << "Dog{}" << endl;
	}

	~Dog() {
		cout << "~Dog" << endl;
	}

	void Sound() const {
		cout << (weight < 10 ? "bark" : "BARK") << endl;
	}

	friend ostream& operator<<(ostream& otp, const Dog& d) {
		return otp << "Dog[weight=" << d.weight << "; age=" << d.age << "]";
	}

};

class Spaniel : public Dog {
	string color;

public:
	Spaniel(float weight, float age, string color)
		: Dog{ weight, age }, color{ color }
	{
		cout << "Spaniel{}" << endl;
	}

	~Spaniel() {
		cout << "~Spaniel" << endl;
	}

	friend ostream& operator<<(ostream& otp, const Spaniel& d) {
		return otp << "Spaniel[color=" << d.color << "; " << static_cast<const Dog&>(d) << "]";
	}

};

void Test(const Spaniel& d) {
	cout << d << endl;
	d.Sound();
}

int main()
{
	Test(Spaniel{ 15, 2, "red" });
	Test(Spaniel{ 5, 1, "brown" });
}