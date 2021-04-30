#pragma once



template <class T>
class List {
private:
	T* els;
	size_t len;

public:

	List() {}
	List(List&& l) noexcept
		: els(l.els)
		, len(l.len)
	{
		l.els = nullptr;
	}

	~List() {
		delete[] els;
		len = -1;
	}

	auto Count() {
		return len;
	}

	void Clear() {
		delete[] els;
		len = 0;
	}

#pragma region Add

	template <class T2 = T&&>
	auto& AddAt(T2&& el, int ind) {
		if ((size_t)ind > len)
			throw std::out_of_range("ToDo"); //ToDo
		++len;

		auto n_els = new T[len];
		for (size_t i = 0; i < (size_t)ind; ++i) n_els[i] = std::move(els[i]);
		n_els[ind] = std::move(el);
		for (size_t i = ind + 1; i < len; ++i) n_els[i] = std::move(els[i - 1]);

		delete[] els;
		els = n_els;

		return *this;
	}

	template <class T2 = T&&>
	auto& Add(T2&& el) {
		return AddAt(el, len);
	}
	template <class T2 = T&&>
	auto& operator<<(T2&& el) {
		return Add(std::forward<T2>(el));
	}

#pragma endregion

#pragma region Remove

	template <class TProc>
	auto& RemoveWhere(TProc p) {
		int ins_ind = 0;

		for (size_t i = 0; i < len; ++i)
		{
			if (p(els[i])) continue;

			if (i != ins_ind)
				els[ins_ind] = std::move(els[i]);
			++ins_ind;
		}

		len = ins_ind;
		return *this;
	}

	auto& Remove(const T& el) {
		return RemoveWhere([&](T& x) { return x == el; });
	}

	auto& RemoveAt(int ind) {
		if ((unsigned)ind >= len)
			throw std::out_of_range("ToDo"); //ToDo

		for (int i = ind + 1; i < len; ++i)
			els[i - 1] = std::move(els[i]);

		--len;
		return *this;
	}

#pragma endregion

#pragma region Sort

	template <class TComp>
	auto& Sort(TComp comp) {

		for (size_t i1 = 0; i1 < len; ++i1)
			for (size_t i2 = 0; i2 < len - i1; ++i2)
				if (comp(els[i1], els[i2]) < 0)
					std::swap(els[i1], els[i2]);

		return *this;
	}

#pragma endregion

#pragma region Iteratorable

	auto begin() const {
		return els;
	}

	auto end() const {
		return els+len;
	}

#pragma endregion

};



