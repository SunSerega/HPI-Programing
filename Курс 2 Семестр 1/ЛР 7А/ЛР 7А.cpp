#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Complex {
	double r, i;

public:
	Complex(double r, double i)
		: r{ r }
		, i{ i }
	{}

	void Write(ofstream& otp) {
		otp << "Complex(" << r << " + i*" << i << ")" << endl;
	}

};

int main()
{
	vector<Complex> v{};

	ifstream inp{ "inp.txt" };
	while (true) {
		double r, i;
		if (!(inp >> r >> i)) break;
		v.push_back({ r,i });
	}
	inp.close();

	ofstream otp{ "otp.txt" };
	for (auto& c : v)
		c.Write(otp);
	otp.close();

}