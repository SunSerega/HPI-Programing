#include <iostream>
#include <iomanip>
using namespace std;

ostream& manip1(ostream& stream)
{
	stream.setf(ios::oct, ios::basefield);
	stream.setf(ios::uppercase);
	return stream;
}

ostream& manip2(ostream& stream)
{
	stream.unsetf(ios::uppercase);
	stream.fill('#');
	return stream;
}

ostream& manip3(ostream& stream)
{
	stream.setf(ios::hex, ios::basefield);
	return stream;
}
istream& manip3(istream& stream)
{
	stream.setf(ios::skipws);
	return stream;
}

int main()
{

	cout << manip1 << 100 << ' ' << hex << 255 << endl;

	cout << manip2 << setw(5) << hex << 255 << endl;

	cout << manip3 << 100 << endl;
	char ch1, ch2;
	cin >> noskipws >> ch1;
	cin >> manip3 >> ch2;
	cout << "\"" << ch1 << "\", \"" << ch2 << "\"" << endl;

}