#pragma once

#include <string>
#include "LList.h"
#include "BinaryFiles.h"

class ProductData
{
private:
	std::wstring name;
	std::wstring producer;
	int part_count;
	int cost_in_dollars;

	friend std::wostream& operator<<(std::wostream& os, const LList<ProductData>& data);
	friend std::wistream& operator>>(std::wistream& is, ProductData& data);

public:
	//ToDo move to private or delete
	const ProductData() noexcept
		: name()
		, producer()
		, part_count()
		, cost_in_dollars()
	{}

	const ProductData(std::wstring name, std::wstring producer, int part_count, int cost_in_dollars) noexcept
		: name(std::move(name))
		, producer(std::move(producer))
		, part_count(part_count)
		, cost_in_dollars(cost_in_dollars)
	{}

	ProductData(ProductData& d) noexcept
		: name(d.name)
		, producer(d.producer)
		, part_count(d.part_count)
		, cost_in_dollars(d.cost_in_dollars)
	{}

	ProductData(ProductData&& d) noexcept
		: name(std::move(d.name))
		, producer(std::move(d.producer))
		, part_count(d.part_count)
		, cost_in_dollars(d.cost_in_dollars)
	{}

	bool operator==(const ProductData& other) {
		if (this->name != other.name) return false;
		if (this->producer != other.producer) return false;
		if (this->part_count != other.part_count) return false;
		if (this->cost_in_dollars != other.cost_in_dollars) return false;
		return true;
	}

	auto& get_name() {
		return name;
	}
	auto& get_producer() {
		return producer;
	}
	auto& get_part_count() {
		return part_count;
	}
	auto& get_cost_in_dollars() {
		return cost_in_dollars;
	}

	void Serialize(BinaryWriter& bw) {
		bw << name << producer << part_count << cost_in_dollars;
	}
	
	static auto Deserialize(BinaryReader& br) {
		ProductData res;
		br >> res.name >> res.producer >> res.part_count >> res.cost_in_dollars;
		return res;
	}

};

std::wostream& operator<<(std::wostream& os, const LList<ProductData>& data);
std::wistream& operator>>(std::wistream& is, ProductData& data);


