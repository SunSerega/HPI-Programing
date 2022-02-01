#include <iostream>
#include <numeric>
#include <algorithm>
using namespace std;

template <class TItem>
class DynamicArray {
	TItem* data;
	size_t used_size = 0;
	size_t buf_size;

	void InitBuf(size_t buf_size) {
		this->buf_size = buf_size;
		this->data = static_cast<TItem*>(operator new[](buf_size * sizeof(TItem)));
	}

private:
	class count_t {
		DynamicArray& source;
		count_t(DynamicArray& source) : source{ source } {}
		count_t(count_t& prev) = delete;

		friend class DynamicArray;
	public:
		operator size_t() const { return source.used_size; }

	};
public:
	const count_t count{ *this };

private:
	class capacity_t {
		DynamicArray& source;
		capacity_t(DynamicArray& source) : source{ source } {}
		capacity_t(capacity_t& prev) = delete;

		friend class DynamicArray;
	public:
		operator size_t() const { return source.buf_size; }

		size_t operator=(size_t new_buf_size) const {
			auto old_data = source.data;
			auto old_buf_size = source.buf_size;
			if (new_buf_size)
				source.InitBuf(new_buf_size);
			if (old_buf_size) {
				if (new_buf_size)
					memcpy(source.data, old_data, min(old_buf_size, new_buf_size) * sizeof(TItem));
				operator delete[](old_data);
			}
			return new_buf_size;
		}

	};
public:
	const capacity_t capacity{ *this };

public:
	using iter = TItem*;

	DynamicArray() {
		capacity = 100;
	}
	DynamicArray(size_t cap) {
		capacity = cap;
	}

	template <class ...ArgT>
	iter emplace(ArgT&&... arg) {
		if (used_size == buf_size) capacity = capacity * 2;
		iter res = &data[used_size++];
		new (res) TItem(forward<ArgT>(arg)...);
		return res;
	}

	auto& operator[](size_t ind) {
		if (ind >= count) throw ind;
		return data[ind];
	}

	auto begin() const {
		return data;
	}
	auto end() const {
		return data + count;
	}
	friend ostream& operator<<(ostream& otp, const DynamicArray& a) {
		copy(a.begin(), a.end(), ostream_iterator<int>(otp, " "));
		return otp;
	}

	auto sum() {
		return accumulate(begin(), end(), size_t(0));
	}
	auto avg() {
		return sum() / double(count);
	}

	auto max() {
		class {
			friend auto DynamicArray::max();
			DynamicArray* source;
		public:
			auto& operator[](size_t len) {
				if (len > source->count) throw len;
				return *max_element(source->begin(), source->begin() + len);
			}
		} res{};
		return (res.source = this, res);
	}

	~DynamicArray() {
		for (auto& x : *this)
			x.~TItem();
		operator delete[](data);
	}

};

int main()
{
	DynamicArray<int> a{};
	for (size_t i = 1; i < 90; ++i) a.emplace(int(i*i));

	cout << a.count << '/' << a.capacity << ':' << endl;
	cout << a << endl;
	cout << a.sum() << ' ' << a.avg() << endl;
	cout << a.max()[10] << endl;

}