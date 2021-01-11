#include <iostream>
#include <io.h>
#include <fcntl.h>

#include <type_traits>
#include <array>
#include <chrono>
#include <windows.h>

#include "local_utils.h"

using namespace std;
using namespace std::chrono;

#define TEST_COUNT 10000

namespace ConsoleCursor {
	HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD GetCoord() {
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		if (GetConsoleScreenBufferInfo(hnd, &cbsi))
			return cbsi.dwCursorPosition;
		COORD invalid{};
		return invalid;
	}

	void Teleport(int X, int Y) {
		SetConsoleCursorPosition(hnd, COORD{ (SHORT)X, (SHORT)Y });
	}

	void TeleportOn(int dX, int dY) {
		auto pos = GetCoord();
		Teleport(pos.X + dX, pos.Y + dY);
	}

	struct {
		int& operator = (int& new_x) {
			Teleport(new_x, GetCoord().Y);
			return new_x;
		}
		operator int() { return GetCoord().X; }
	} X;

	struct {
		int& operator = (int& new_y) {
			Teleport(GetCoord().X, new_y);
			return new_y;
		}
		operator int() { return GetCoord().Y; }
	} Y;

}

template <class CALLABLE>
struct test_info {
	CALLABLE proc;

	test_info(CALLABLE proc) {
		this->proc = proc;
	}

	template <class TData>
	auto Execute(TData a[], int len) {
		nanoseconds res{};
		auto data = new TData[len];

		for (auto test = 0; test < TEST_COUNT; ++test)
		{
			for (auto i = 0; i < len; ++i) data[i] = a[i];
			auto start = high_resolution_clock::now();

			proc(data, len);

			auto stop = high_resolution_clock::now();
			res += stop - start;
		}

		delete[] data;
		return duration_cast<milliseconds>( res ).count();
	}

};

template <class ... CALLABLE>
auto CombineTestsArr(CALLABLE ... args) {
	using common_t = common_type_t<CALLABLE...>;
	return array<test_info<common_t>, sizeof...(CALLABLE)> { test_info<common_t>(args)... };
}

namespace SortAlgorithms
{

	namespace Radix {

		template <class TData>
		void CountSort(TData data[], size_t len, TData temp[], int base, int exp) {
			auto count = new int[base] {};

			for (size_t i = 0; i < len; ++i)
				// Вообще это не будет работать для отрицательных значений...
				// Для CountSort нужна функция ключа, возвращающая только не_отрицательные значения
				// Но в данном случае значения генерируются с помощью "rand() % ...", поэтому они всегда не_отрицательные
				++count[data[i] / exp % base];

			for (int i = 1; i < base; ++i)
				count[i] += count[i - 1];

			// Перечисление в обратном порядке, чтоб не менять местами элементы с одинаковой цифрой в данном порядке
			for (int i = len - 1; i >= 0; --i)
				temp[--count[data[i] / exp % base]] = data[i];

			for (size_t i = 0; i < len; ++i) data[i] = temp[i];
		}

	}

	template <class TData>
	void RadixSort(TData data[], size_t len) {
		size_t base = len;

		decltype(base) m = data[0];
		for (size_t i = 1; i < len; ++i)
			m = max(m, static_cast<decltype(base)>(data[i]) );

		auto temp = new TData[len];

		for (decltype(base) exp = 1; exp <= m; exp *= base)
			Radix::CountSort(data, len, temp, base, exp);

		delete[] temp;
	}

	namespace Tree {

		template <class TData>
		class Node {
		private:
			TData data;
			Node* l = NULL;
			Node* r = NULL;

		public:
			Node(TData data) : data(data) {};

			void Add(TData new_data) {
				Node*& branch = new_data < data ? l : r;

				if (branch)
					branch->Add(new_data);
				else
					branch = new Node(new_data);

			}

			static Node* MakeTree(TData a[], size_t len) {
				auto res = new Node(a[0]);
				for (size_t i = 1; i < len; ++i) res->Add(a[i]);
				return res;
			}

			void StoreTo(TData*& a) {
				if (l) l->StoreTo(a);
				*(a++) = data;
				if (r) r->StoreTo(a);
			}

			~Node() {
				if (l) delete l;
				if (r) delete r;
			}

		};

	}

	namespace Tree2 {

		template <class TData>
		class Node {
		private:
			TData data;
			Node* l = NULL;
			Node* r = NULL;

			Node() : data() {};

		public:
			Node(TData data) : data(data) {};

			void Add(TData new_data, Node*& last_node) {
				auto _this = this;
				Node** el = &_this;

				while (*el) el = new_data < (**el).data ? &(**el).l : &(**el).r;

				*(*el = ++last_node) = Node(new_data);
			}

			static Node* MakeTree(TData a[], size_t len) {
				auto res = new Node[len];
				res[0] = Node(a[0]);

				auto last_node = res;
				for (size_t i = 1; i < len; ++i)
					res->Add(a[i], last_node);

				return res;
			}

			void StoreTo(TData*& a) {
				if (l) l->StoreTo(a);
				*(a++) = data;
				if (r) r->StoreTo(a);
			}

		};

	}

	template <class TData>
	void TreeSort(TData data[], size_t len) {
		auto tree = Tree2::Node<TData>::MakeTree(data, len);

		tree->StoreTo(data);
		// Следущее понадобится только если data
		// будет использоваться в этой функции ниже
		// Или, лучше, посылать в StoreTo отдельную переменную temp
//		data -= len;

		// [] только для Tree2
		delete[] tree;
	}

	template <class TData>
	auto AllTestsArr = CombineTestsArr(
			&RadixSort	<TData>,
			&TreeSort	<TData>
		);

}

#define test_min_size 20
#define test_step_size 20
#define test_max_size 300

template <class TData>
void TestIteration(TData a[], size_t len) {
	auto &tests = SortAlgorithms::AllTestsArr<TData>;
	int x = ConsoleCursor::X;
	int y = ConsoleCursor::Y;

	wcout << len;
	int max_w = ConsoleCursor::X - x;

	for (size_t i = 0; i < tests.size(); ++i)
	{
		ConsoleCursor::Teleport(x, y + i + 1);

		auto data = new TData[len];
		for (size_t i2 = 0; i2 < len; ++i2) data[i2] = a[i2];

		wcout << tests[i].Execute(data, len);
		max_w = max(max_w, ConsoleCursor::X - x);
	}

	ConsoleCursor::Teleport(x + max_w + 1, y);
}

template <class TData>
void AllTestIterations(TData a[]) {
	int x = ConsoleCursor::X;
	int y = ConsoleCursor::Y;

	for (size_t len = test_min_size; len <= test_max_size; len += test_step_size)
		TestIteration<TData>(a, len);

	ConsoleCursor::Teleport(x, y + SortAlgorithms::AllTestsArr<TData>.size());
}

template <class TData>
void Println(TData a[], size_t len) {
	for (size_t i = 0; i < len; ++i)
		wcout << a[i] << L' ';
	wcout << endl;
}

int main() {
	if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
	if (_setmode(_fileno(stdin ), _O_U16TEXT) == -1) return -1;
	if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;



	auto data = new int[test_max_size];
	for (auto i = 0; i < test_max_size; ++i)
		data[i] = rand() % 500;

	wcout << L"Не отсортированный массив:" << endl;
	AllTestIterations(data);
	wcout << endl << endl;



	auto sorted = new int[test_max_size];
	for (auto i = 0; i < test_max_size; ++i) sorted[i] = data[i];
	SortAlgorithms::RadixSort(sorted, test_max_size);

	wcout << L"Отсортированный массив:" << endl;
	AllTestIterations(sorted);
	wcout << endl << endl;



	auto reversed = new int[test_max_size];
	for (auto i = 0; i < test_max_size; ++i) reversed[i] = sorted[test_max_size - i - 1];

	wcout << L"Отсортированный в обратном порядке массив:" << endl;
	AllTestIterations(reversed);
	wcout << endl << endl;



	{
		size_t const integrity_test_size = 20;

		wcout << L"Тестирование адекватности сортировок:" << endl;
		Println(data, integrity_test_size);

		auto temp = new int[integrity_test_size];

		for (auto t : SortAlgorithms::AllTestsArr<int>)
		{
			for (size_t i = 0; i < integrity_test_size; ++i) temp[i] = data[i];
			t.proc(temp, integrity_test_size);
			Println(temp, integrity_test_size);
		}

		delete[] temp;
	}



	delete[] data;
	delete[] sorted;
	delete[] reversed;
}