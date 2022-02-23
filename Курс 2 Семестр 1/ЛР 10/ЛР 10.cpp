#include <iostream>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <concepts>
using namespace std;

template <class T, enable_if_t<(sizeof(T) > sizeof(void*)), int> = 0>
auto min_f(const T& a, const T& b) {
	return a < b ? a : b;
}
template <class T, enable_if_t<(sizeof(T) <= sizeof(void*)), int> = 0>
auto min_f(const T a, const T b) {
	return a < b ? a : b;
}

template <ranges::random_access_range TRange>
void sort(TRange& range) {
	auto b = begin(range);
	auto e = end(range);

	while (b != e) {
		auto iter = b;

		while (true) {
			auto next = iter + 1;
			if (next == e) break;
			if (*next < *iter) swap(*iter, *next);
			iter = next;
		}

		e = iter;
	}

}

template <ranges::range TRange,
	class TPred = decltype([](ranges::range_value_t<TRange>) { return true; }),
	class TRes = decay_t<ranges::range_value_t<TRange>>
> TRes sum(const TRange& range, TPred pred = {}) {
	TRes res{};
	for (auto& x : range)
		if (pred(x))
			res += x;
	;
	return res;
}

int main()
{

	{
		int a1 = 1, b1 = 2;
		assert(("min(int)1: ", min_f(a1, b1) == a1));
		assert(("min(int)2: ", min_f(b1, a1) == a1));
		string a2 = "abc", b2 = "def";
		assert(("min(string)1: ", min_f(a2, b2) == a2));
		assert(("min(string)2: ", min_f(b2, a2) == a2));
	}

	{
		int x[]{ 5, 4, 3, 2, 1 };
		sort(x);
		assert(("sort: ", std::ranges::equal(x, decltype(x){1,2,3,4,5})));
	}

	{
		int x[]{ -2,-1,0,+1,+2 };
		assert(("sum1: ", sum(x) == 0));
		assert(("sum2: ", sum(x, [](auto x) {return x > 0; }) == 3));
	}

}