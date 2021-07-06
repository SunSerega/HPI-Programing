#pragma once

template <size_t c, class T>
struct Vec {
	T val[c];

#pragma region Add

	template <class T2>
	auto operator+(const T2 scalar) const {
		Vec<c, decltype(std::declval<T>() + std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] + scalar;
		return res;
	}
	template <class T2>
	auto operator+(const Vec<c, T2> v) const {
		Vec<c, decltype(std::declval<T>() + std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] + v[i];
		return res;
	}
	template <class T2>
	Vec operator+=(const T2 v) {
		Vec res = *this + v;
		*this = res;
		return res;
	}

#pragma endregion

#pragma region Sub

	template <class T2>
	auto operator-(const T2 scalar) const {
		Vec<c, decltype(std::declval<T>() - std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] - scalar;
		return res;
	}
	template <class T2>
	auto operator-(const Vec<c, T2> v) const {
		Vec<c, decltype(std::declval<T>() - std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] - v[i];
		return res;
	}
	template <class T2>
	Vec operator-=(const T2 v) {
		Vec res = *this - v;
		*this = res;
		return res;
	}

#pragma endregion

#pragma region Mul

	template <class T2>
	auto operator*(const T2 scalar) const {
		Vec<c, decltype(std::declval<T>() * std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] * scalar;
		return res;
	}
	template <class T2>
	auto operator*(const Vec<c, T2> v) const {
		Vec<c, decltype(std::declval<T>() * std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] * v[i];
		return res;
	}
	template <class T2>
	Vec operator*=(const T2 v) {
		Vec res = *this * v;
		*this = res;
		return res;
	}

#pragma endregion

#pragma region Div

	template <class T2>
	auto operator/(const T2 scalar) const {
		Vec<c, decltype(std::declval<T>() / std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] / scalar;
		return res;
	}
	template <class T2>
	auto operator/(const Vec<c, T2> v) const {
		Vec<c, decltype(std::declval<T>() / std::declval<T2>())> res;
		for (int i = 0; i < c; ++i)
			res.val[i] = this->val[i] / v[i];
		return res;
	}
	template <class T2>
	Vec operator/=(const T2 v) {
		Vec res = *this / v;
		*this = res;
		return res;
	}

#pragma endregion

#pragma region Misc

	template <class T2>
	operator Vec<c, T2>() {
		Vec<c, T2> res;
		for (int i = 0; i < c; ++i)
			res[i] = val[i];
		return res;
	}

	bool operator==(const Vec& other) const {
		for (int i = 0; i < c; ++i)
			if (this->val[i] != other.val[i])
				return false;
		return true;
	}

	T& operator[] (size_t ind) {
		return val[ind];
	}
	const T& operator[] (size_t ind) const {
		return val[ind];
	}

	T get_min() {
		T res = val[0];
		for (int i = 1; i < c; ++i)
			if (val[i] < res) res = val[i];
		return res;
	}
	T get_max() {
		T res = val[0];
		for (int i = 1; i < c; ++i)
			if (val[i] > res) res = val[i];
		return res;
	}

#pragma endregion

};

namespace std {
	template< class Key >
	struct hash;

	template <size_t c, class T>
	struct hash<Vec<c, T>> {
		size_t operator() (const Vec<c, T>& v) const {
			size_t res = v[0];
			for (size_t i = 1; i < c; ++i)
#pragma warning (push)
#pragma warning (disable: 26451)
				res ^= v.val[i] + 0x9e3779b9 + (res << 6) + (res >> 2);
#pragma warning (pop)
			return res;
		}
	};

}


