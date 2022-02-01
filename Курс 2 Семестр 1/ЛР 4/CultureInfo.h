#pragma once
#include <string>
#include <iostream>

class CultureInfo final {
	std::string name{};
	enum class CultureType { cereal, bean } type{};
	int area_in_use{};
	float efficiency{};

public:
	CultureInfo(std::istream& stream) {
		stream >> *this;
	}

	const std::string& get_name() const { return name; };

	friend std::istream& operator>>(std::istream& stream, CultureInfo& p);

	friend std::ostream& operator<<(std::ostream& stream, const CultureInfo& p);

};
