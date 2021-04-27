#pragma once

#include <fstream>
#include <stdexcept>
#include <string>



class BinaryWriter {
private:
	std::ofstream str;
	
#pragma region saveable_info

	template <class T, class T2 = void>
	struct saveable_info {
		static const auto is_bad = false;
		static const auto is_bit_block = true;
		static const auto is_serializeable = false;
	};
	template <class T>
	struct saveable_info<T*> {
		static const auto is_bad = true;
		static const auto is_bit_block = false;
		static const auto is_serializeable = false;
	};
	template <class T>
	struct saveable_info<T, decltype(std::declval<T>().Serialize(std::declval<BinaryWriter&>()), (void)0)> {
		static const auto is_bad = false;
		static const auto is_bit_block = false;
		static const auto is_serializeable = true;
	};

#pragma endregion

public:
	BinaryWriter(const std::wstring& fname) : str(fname, std::ios::binary) {}
	BinaryWriter(std::ofstream&& str) noexcept : str(std::move(str)) {}
	BinaryWriter(BinaryWriter&& bw) noexcept : str(std::move(bw.str)) {}

#pragma region Write

	template <class T, std::enable_if_t<saveable_info<T>::is_bad, bool> = true>
	inline void Write(T&& el) {
		static_assert(false, "Bad template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_bit_block, bool> = true>
	inline void Write(T&& el) {
		str.write((const char*)&el, sizeof(T));
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_serializeable, bool> = true>
	inline void Write(T&& el) {
		el.Serialize(*this);
	}

	inline void Write(std::wstring& s) {
		Write7bit(s.length());
		str.write((char*)&s[0], (std::streamsize)s.length() * sizeof(wchar_t));
	}

#pragma endregion

#pragma region Write7bit

	template <class T, std::enable_if_t<saveable_info<T>::is_bad, bool> = true>
	inline void Write7bit(T el) {
		static_assert(false, "Bad template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_serializeable, bool> = true>
	inline void Write7bit(T el) {
		static_assert(false, "Serializable template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_bit_block, bool> = true>
	inline void Write7bit(T el) {
		constexpr int saved_bits = 7;
		constexpr int len = (sizeof(T) * 8 - 1) / saved_bits + 1;
		constexpr char last_bit = (char)(1 << saved_bits);
		constexpr char body_mask = (char)((int)last_bit - 1);

		char buff[len];
		char* buff_tip = buff;

		do {
			char val = el & body_mask;
			el = el >> 7;
			val |= el ? 0 : last_bit;
			*buff_tip = val;
			++buff_tip;
		} while (el);

		str.write(buff, buff_tip - buff);
	}

#pragma endregion

#pragma region WriteBlock

	template <class T, std::enable_if_t<saveable_info<T>::is_bad, bool> = true>
	inline void WriteBlock(T els[], int len) {
		static_assert(false, "Bad template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_serializeable, bool> = true>
	inline void WriteBlock(T els[], int len) {
		static_assert(false, "Block of serializable types");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_bit_block, bool> = true>
	inline void WriteBlock(T els[], int len) {
		str.write((char*)els, (std::streamsize)len * sizeof(T));
	}

#pragma endregion

#pragma region operator<<

	template <class T, class T2 = std::enable_if_t<!std::is_integral_v<std::remove_reference_t<T>> || sizeof(T) == 1, void>>
	inline BinaryWriter& operator<<(T&& el) {
		Write(std::forward<T>(el));
		return *this;
	}

	template <class T, class T2 = std::enable_if_t<std::is_integral_v<std::remove_reference_t<T>> && sizeof(T) != 1, void>>
	inline BinaryWriter& operator<<(T el) {
		Write7bit(el);
		return *this;
	}

#pragma endregion

	void Flush() {
		str.flush();
	}

	void Close() {
		str.close();
	}

};



class BinaryReader {
private:
	std::ifstream str;

#pragma region saveable_info

	template <class T, class T2 = void>
	struct saveable_info {
		static const auto is_bit_block = true;
		static const auto is_deserializeable = false;
		static const auto is_bad = false;
	};
	template <class T>
	struct saveable_info<T*> {
		static const auto is_bit_block = false;
		static const auto is_deserializeable = false;
		static const auto is_bad = true;
	};
	template <class T>
	struct saveable_info<T, decltype(T::Deserialize(std::declval<BinaryReader&>()), (void)0)> {
		static const auto is_bit_block = false;
		static const auto is_deserializeable = true;
		static const auto is_bad = false;
	};

#pragma endregion

public:
	BinaryReader(const std::wstring& fname) : str(fname, std::ios::binary) {}
	BinaryReader(std::ifstream&& str) noexcept  : str(std::move(str)) {}
	BinaryReader(BinaryReader&& br) noexcept : str(std::move(br.str)) {}

#pragma region Read

	template <class T, std::enable_if_t<saveable_info<T>::is_bad, bool> = 0>
	inline void Read() {
		static_assert(false, "Bad template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_bit_block, bool> = 0>
	inline T Read() {
		T res;
		str.read((char*)&res, sizeof(T));
		return res;
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_deserializeable, bool> = 0>
	inline T Read() {
		return T::Deserialize(*this);
	}

	template <>
	std::wstring Read<std::wstring>() {
		auto len = Read7bit<int>();

		std::wstring res(len, L'\0');
		str.read((char*)&res[0], len*sizeof(wchar_t));

		return res;
	}

#pragma endregion

#pragma region Read7bit

	template <class T, std::enable_if_t<saveable_info<T>::is_bad, bool> = true>
	inline void Read7bit(size_t len) {
		static_assert(false, "Bad template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_deserializeable, bool> = true>
	inline T Read7bit(size_t len) {
		static_assert(false, "Deserializable template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_bit_block, bool> = true>
	inline T Read7bit() {
		constexpr int saved_bits = 7;
		constexpr char last_bit = (char)(1 << saved_bits);
		constexpr char body_mask = (char)((int)last_bit - 1);

		T res{};
		int bit_ind = 0;
		while (true) {
			char val;
			str.read(&val, 1);
			res |= (val & body_mask) << bit_ind;
			if (val & last_bit) break;
			bit_ind += 7;
		}

		return res;
	}

#pragma endregion

#pragma region ReadBlock

	template <class T, std::enable_if_t<saveable_info<T>::is_bad, bool> = true>
	inline void ReadBlock(size_t len) {
		static_assert(false, "Bad template type");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_deserializeable, bool> = true>
	inline T ReadBlock(size_t len) {
		static_assert(false, "Block of deserializable types");
	}

	template <class T, std::enable_if_t<saveable_info<T>::is_bit_block, bool> = true>
	inline T* ReadBlock(size_t len) {
		auto res = new T[len];
		str.read((char*)res, sizeof(T)*len);
		return res;
	}

#pragma endregion

#pragma region operator>>

	template <class T, std::enable_if_t<!(std::is_integral_v<std::remove_reference_t<T>> && sizeof(T) != 1), bool> = true>
	BinaryReader& operator>>(T& el) {
		el = Read<T>();
		return *this;
	}

	template <class T, std::enable_if_t<std::is_integral_v<std::remove_reference_t<T>> && sizeof(T) != 1, bool> = true>
		BinaryReader& operator>>(T& el) {
		el = Read7bit<T>();
		return *this;
	}

#pragma endregion

	int Peek() {
		return str.peek();
	}

	bool ReachedEOF() {
		return str.eof();
	}

	void Close() {
		str.close();
	}

};


