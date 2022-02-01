#pragma once
#include <string>
#include <iostream>

class CultureInfo final {
public:
	enum class CultureType { cereal, bean };
private:
	std::string name;
	CultureType type;
	int area_in_use;
	float efficiency;

public:
	CultureInfo(std::string name, CultureType type, int area_in_use, float efficiency)
		: name{ name }
		, type{ type }
		, area_in_use{ area_in_use }
		, efficiency{ efficiency }
	{}

#pragma warning( disable : 26495 )
	CultureInfo(std::istream& stream)
		: name{}
	{
		stream >> *this;
	}
#pragma warning( default : 26495 )

	const auto& get_name() const { return name; };
	const auto& get_type() const { return type; };
	const auto& get_area() const { return area_in_use; };
	const auto& get_effi() const { return efficiency; };

	friend std::istream& operator>>(std::istream& stream, CultureInfo& p);

	friend std::ostream& operator<<(std::ostream& stream, const CultureInfo& p);

};
