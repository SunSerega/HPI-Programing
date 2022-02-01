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

	// 1)
	// Нет такого манипулятора, чтобы все символы были большими.
	// "uppercase" влияет только на вывод hex чисел 0XAF vs 0xaf
	// А это не сочетается с oct выводом, поэтому очень странное задание на manip1
	cout << manip1 << 100 << ' ' << hex << 255 << endl;

	// 2)
	// Опять же, "nouppercase" влияет только на hex вывод
	// И "setfill" влияет только на пробелы, которые создаёт setw, а не на все
	cout << manip2 << setw(5) << hex << 255 << endl;

	// 3)
	// А тут вообще не понятно - hex это манипулятор вывода, а ws - манипулятор ввода
	// При этом в задании сказано объеденить их в общий манипулятор
	// Пока сделал 2 манипулятора с общим именем, но что то с этим задание не так
	cout << manip3 << 100 << endl;
	char ch1, ch2;
	// Кроме того, игнор пробелов (ws) включён всегда по-умолчанию, то есть его ещё надо сначала выключить для тестирования
	cin >> noskipws >> ch1;
	cin >> manip3 >> ch2;
	cout << "\"" << ch1 << "\", \"" << ch2 << "\"" << endl;

}