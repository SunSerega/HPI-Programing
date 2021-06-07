#pragma once

template <size_t c, class T>
struct Vec {
	T val[c];

	Vec operator-(const Vec& other) const {
		Vec res{};
		for (int i = 0; i < c; ++i)
			res[i] = this->val[i] - other.val[i];
		return res;
	}

	Vec operator+=(const Vec& offset) {
		for (int i = 0; i < c; ++i)
			this->val[i] += offset.val[i];
		return *this;
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


