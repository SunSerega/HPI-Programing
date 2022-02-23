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

	friend bool operator<(const CultureInfo& p1, const CultureInfo& p2) {
		return p1.name < p2.name;
	}

	friend std::istream& operator>>(std::istream& stream, CultureInfo& p);

	friend std::ostream& operator<<(std::ostream& stream, const CultureInfo& p);

};
