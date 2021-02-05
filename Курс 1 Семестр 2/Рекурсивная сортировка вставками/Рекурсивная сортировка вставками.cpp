#include <iostream>

using namespace std;

template <class T>
void RecInsertSort(T arr[], int len) {
	if (len <= 1) return;
	RecInsertSort(arr + 1, len - 1);
	while (--len)
	{
		if (arr[0] <= arr[1]) return;
		swap(arr[0], arr[1]);
		++arr;
	}
}

int main()
{

	cout << "Enter length: ";
	int len;
	cin >> len;

	cout << "Enter " << len << " elements, space-separated:\n";
	auto arr = new int[len];
	for (auto i = 0; i < len; ++i)
		cin >> arr[i];

	RecInsertSort(arr, len);

	cout << "After sorting:\n";
	for (auto i = 0; i < len; ++i)
		cout << arr[i] << ' ';
	cout << '\n';

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
