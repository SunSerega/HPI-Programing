#include <iostream>
#include <fstream>
using namespace std;

const int len = 10;
const int split_ind = 5 - 1;
const int file_mode = ios::in | ios::out | ios::binary;

void GenInp() {
	ofstream gen{ "inp.bin", file_mode | ios::trunc };

	for (int i = 1; i <= 10; ++i)
	{
		gen.write((char*)&i, sizeof(i));
	}

	gen.close();
}

int main()
{
	GenInp();
	fstream f{ "inp.bin", file_mode };
	if (!f) throw 0;

	bool odd;
	{
		f.seekg(split_ind * sizeof(int));
		int fifth;
		if (!f.read((char*)&fifth, sizeof(fifth))) throw 0;
		odd = fifth & 1;
	}

	if (odd) {
		f.seekp((split_ind + 1) * sizeof(int));
		int temp = 88;
		for (int i = split_ind + 1; i < len; ++i)
			f.write((char*)&temp, sizeof(temp));
	}
	else {
		f.seekp(0);
		int temp = 77;
		for (int i = 0; i < split_ind; ++i)
			f.write((char*)&temp, sizeof(temp));
	}
	if (!f) throw 0;

	f.seekp(0);
	while (true) {
		int temp;
		f.read((char*)&temp, sizeof(temp));
		if (f.eof()) break;
		if (!f) throw 0;
		cout << temp << ' ';
	}

	f.close();
}