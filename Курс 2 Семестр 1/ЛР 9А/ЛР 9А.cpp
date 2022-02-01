#include <iostream>
#include <functional>
using namespace std;

template <class TItem, size_t size> requires (size > 0)
class vec {
	char mem[sizeof(TItem) * size];

	template <class TFirst, class ... Ts>
	static inline void InitItems_val(TItem* items, TFirst first, Ts ... rest) {
		new(items) TItem(first);
		InitItems_val(items + 1, rest...);
	}
	static void InitItems_val(TItem* items) {};

	template <size_t c>
	struct FuncIniter {
		template <class TInit>
		static inline void InitItems(TItem* items, function<TInit(size_t)>& func) {
			FuncIniter<c - 1>::InitItems(items, func);
			new(items+c) TItem(func(c));
		}
	};
	template <>
	struct FuncIniter<-1> {
		template <class TInit>
		static void InitItems(TItem* items, function<TInit(size_t)>& func) {}
	};

public:
	template <class ... Ts>
	vec(Ts ... init) requires (sizeof...(Ts) == size) {
		InitItems_val(begin(), init...);
	}

	template <class TInit>
	vec(function<TInit(size_t)>& func) {
		FuncIniter<size - 1>::InitItems(begin(), func);
	}
	template <class TInit>
	vec(function<TInit(size_t)>&& func) : vec(func) {};

	auto begin() {
		return reinterpret_cast<TItem*>(&mem[0]);
	}
	auto begin() const {
		return reinterpret_cast<const TItem*>(&mem[0]);
	}

	auto end() const {
		return begin() + size;
	}

	friend partial_ordering operator<=>(vec& v1, vec& v2) {
		auto iter1 = v1.begin();
		auto iter2 = v2.begin();
		auto res = *iter1++ <=> *iter2++;

		auto c = size;
		while (--c) {
			auto res2 = *iter1++ <=> *iter2++;
			if (res2 == partial_ordering::unordered) continue;
			if (res2 == res) continue;
			if (res == partial_ordering::unordered || res == partial_ordering::equivalent)
				res = res2;
			else if (res2 != partial_ordering::equivalent)
				// Some >, some <
				return partial_ordering::unordered;
		}

		return res;
	}

	friend ostream& operator<<(ostream& otp, const vec& v) {
		copy(v.begin(), v.end(), ostream_iterator<TItem>(otp, " "));
		return otp;
	}

};

template <class T>
class Sum {
	T x, y;

public:
	Sum(T x, T y)
		: x{x}, y{y}
	{}

	auto operator()() const {
		return x + y;
	}

	friend auto operator<=>(Sum s1, Sum s2) {
		return s1() <=> s2();
	}

	friend ostream& operator<<(ostream& otp, const Sum& s) {
		return otp << s.x << '+' << s.y << '=' << s();
	}

};

ostream& operator<<(ostream& otp, partial_ordering ord) {
	switch (ord._Value) {
		case partial_ordering::less			._Value: return otp << "less";
		case partial_ordering::equivalent	._Value: return otp << "equivalent";
		case partial_ordering::greater		._Value: return otp << "greater";
		case partial_ordering::unordered	._Value: return otp << "unordered";
		default: throw ord;
	}
}

template <class TFunc>
auto MakeVec(TFunc&& f) {
	using TItem = decltype(f(declval<size_t>()));
	return vec<TItem, 2>(function<TItem(size_t)>{f});
}

int main()
{
	srand((unsigned)time(nullptr));

	constexpr auto c = 2;
	auto gen = [](size_t i) { return Sum<int>{ rand() % 2, rand() % 2 }; };
	auto v1 = MakeVec(gen);
	auto v2 = MakeVec(gen);

	cout << v1 << endl;
	cout << v2 << endl;

	cout << "v1<=>v1: " << (v1 <=> v1) << endl;
	cout << "v1<=>v2: " << (v1 <=> v2) << endl;
	cout << "v2<=>v1: " << (v2 <=> v1) << endl;
}