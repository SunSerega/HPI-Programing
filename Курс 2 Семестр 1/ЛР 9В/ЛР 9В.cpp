#include <iostream>
#include <set>
#include <optional>
using namespace std;

class Person {
	optional<string> name;
	optional<int> age;
public:
	Person(optional<string> name = nullopt, optional<int> age = nullopt) : name{ name }, age{ age } {}
	Person(optional<int> age) : age{ age } {}

	const optional<string>& get_name() { return name; }
	const optional<int>& get_age() { return age; }

	friend bool operator<(const Person& p1, const Person& p2) {
		auto res = p1.name < p2.name;
		return res;
	}

	struct AgeComparer {
		bool operator()(const Person& p1, const Person& p2) const {
			return p1.age < p2.age;
		}
	};

	friend bool operator==(const Person& p1, const Person& p2) {
		return (p1.age == p2.age) && (p1.name == p2.name);
	}

	friend ostream& operator<<(ostream& otp, const Person& p) {
		otp << "Person[" << p.name.value_or("?");
		if (p.age) otp << ": " << p.age.value();
		return otp << ']';
	}

};

class Staff {
	multiset<Person> by_name{};
	multiset<Person, Person::AgeComparer> by_age{};

	using internal_found_people = pair<multiset<Person>::const_iterator, multiset<Person>::const_iterator>;

	template <class TSet1, class TSet2>
	class found_people : private internal_found_people {
		TSet1& main;
		TSet2& other;
		found_people(internal_found_people r, TSet1& main, TSet2& other)
			: pair{ r }
			, main{ main }, other{ other }
		{}
		friend Staff;
	public:

		auto begin() { return pair::first; }
		auto end() { return pair::second; }

		void erase() {
			if (begin() == end()) return;
			TSet2 temp(begin(), end());
			auto comp = temp.key_comp();

			auto iter = temp.begin();
			auto iter_end = temp.end();
			while (true) {
				auto [erase_begin,erase_end] = other.equal_range(*iter);

				while (true) {

					for (auto erase_iter = erase_begin; erase_iter != erase_end;)
						if (*erase_iter == *iter)
							erase_iter = other.erase(erase_iter); else
							++erase_iter;

					auto& prev_p = *iter;
					if (++iter == iter_end)
						return (main.erase(begin(), end()), (void)0);
					if (comp(prev_p, *iter)) break;
				}

			}

		}

	};

public:
	Staff() {};

	template <class TInit = Person> requires ( is_same_v<remove_reference_t<TInit>, Person> )
	auto emplace(TInit&& init) {
		by_name.emplace(init);
		by_age.emplace(forward<TInit>(init));
	}

	auto begin() { return by_name.begin(); }
	auto end() { return by_name.end(); }

	found_people<decltype(by_name), decltype(by_age)> operator[](optional<string> name) {
		return { by_name.equal_range(Person{ name }), by_name, by_age };
	}
	found_people<decltype(by_age), decltype(by_name)> operator[](optional<int> age) {
		return { by_age.equal_range(Person{ age }), by_age, by_name };
	}

};

int main()
{
	Staff s;

	s.emplace(Person{});
	s.emplace(Person{ "a" });
	s.emplace(Person{ 1 });
	s.emplace(Person{ "a", 1 });

	cout << "All:" << endl;
	for (auto& p : s)
		cout << p << endl;
	cout << string(30, '=') << endl;

	cout << "All with name 'a':" << endl;
	for (auto& p : s["a"])
		cout << p << endl;
	cout << string(30, '=') << endl;

	cout << "All with age 1:" << endl;
	auto age_1 = s[1];
	for (auto& p : age_1)
		cout << p << endl;
	cout << string(30, '=') << endl;

	age_1.erase();
	cout << "All after erasing last found:" << endl;
	for (auto& p : s)
		cout << p << endl;
	cout << string(30, '=') << endl;

}