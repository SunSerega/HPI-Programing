#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <sstream>
#include <functional>

#include "LList.h"
#include "List.h"
#include "ProductData.h"
#include "Console.h"

using namespace std;

void Header(List<ProductData>& l) {
	Console::Clear();
	wcout << l;
}

#pragma region Menu

class Menu {
private:
	List<ProductData>& l;
	wstring name;
	bool (*proc)(bool) = nullptr;
	LList<Menu> sub_menus{};

	inline void AddSubMenus() {}
	template <class T0>
	inline void AddSubMenus(T0&& el0) {
		sub_menus << forward<T0>(el0);
	}
	template <class T0, class ... Ts>
	inline void AddSubMenus(T0&& el0, Ts&&... sub_menus) {
		this->sub_menus << forward<T0>(el0);
		AddSubMenus(forward<Ts>(sub_menus)...);
	}

public:

	template <class ... Ts>
	Menu(List<ProductData>& l, wstring name, Ts&&... sub_menus)
		: l(l), name(name)
	{
		AddSubMenus(forward<Ts>(sub_menus)...);
	}

	template <class ... Ts>
	Menu(List<ProductData>& l, wstring name, bool (*proc)(bool), Ts&&... sub_menus)
		: l(l), name(name), proc(proc)
	{
		AddSubMenus(forward<Ts>(sub_menus)...);
	}

	Menu(Menu&& m) noexcept
		: l(m.l)
		, name(move(m.name))
		, proc(m.proc)
		, sub_menus(move(m.sub_menus))
	{}

	bool Show() {
		Console::SetTitle(name);
		if (proc && !proc(true)) return false;
		while (true) {
			Console::SetTitle(name);
			if (proc && !proc(false)) return true;

			if (sub_menus.Count()) {
				Header(l);

				unsigned c = 0;
				for (auto& m : sub_menus)
					wcout << ++c << L". " << m.name << endl;
				wstring choice = Console::Read<wstring>();

				bool choice_is_int;
				int choice_i;
				try
				{
					choice_i = stoi(choice);
					choice_is_int = true;
				}
				catch (const std::invalid_argument&)
				{
					choice_is_int = false;
				}
				if (choice_is_int && !choice_i) return true;

				auto invalid_choice = true;
				if (choice_is_int && (unsigned)(choice_i - 1) < c)
				{
					auto itr = sub_menus.begin();
					while (--choice_i) ++itr;
					if (!(*itr).Show()) return true;
					invalid_choice = false;
				}
				else for (auto& m : sub_menus)
				{
					if (m.name == choice)
					{
						if (!m.Show()) return true;
						invalid_choice = false;
						break;
					}
				}

				if (invalid_choice)
				{
					wcout << L"Неправильный выбор!";
					wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
				}
			}

		}
	}

};

#pragma endregion

auto l = List<ProductData>();

bool ReadElements(bool) {
	auto ind = Console::Read<int>(L"Введите позицию вставки или -1: ");
	while (true) {
		Header(l);

		ProductData pd;
		if (wcin >> pd)
		{
			if (ind < 0)
				l << move(pd); else
				l.AddAt(move(pd), ind);
		} else
		{
			wcin.clear();
			return false;
		}

	}
}

bool GenerateElements(bool) {
	auto c = Console::Read<size_t>(L"Количество: ");
	while (c--) {
		auto rng_string = []() {
			int c = 3;
			while (rand() % 2) c++;
			auto res = wstring(c, 0);
			for (auto& ch : res)
				ch = rand() % (L'z' - L'a' + 1) + L'a';
			return res;
		};

		l << ProductData{
			rng_string(),
			rng_string(),
			rand() % 1000,
			rand() % 1000,
		};
	}
	return false;
}

bool LoadElements(bool) {
	auto fname = Console::Read<wstring>(L"Имя файла (откуда загружать): ");
	auto br = BinaryReader(fname);

	auto c = br.Read7bit<int>();
	while (c--)
		l << br.Read<ProductData>();

	return false;
}

List<ProductData> sub_l;
bool CopyList(bool first) {
	if (first) {
		sub_l.Clear();
		for (auto& d : l)
			sub_l << d;
	}
	return sub_l.Count();
}

bool SaveElements(bool first) {
	if (!first) return false;
	auto fname = Console::Read<wstring>(L"Имя файла (куда сохранять): ");
	auto bw = BinaryWriter(fname);

	bw.Write7bit(l.Count());
	for (auto& pd : l)
		bw << pd;

	return true;
}

template <class TProc>
void UseWords(const wstring& l, TProc p) {
	wstringstream arg{ L"" };
	wstringstream val{ L"" };
	auto last = &arg;
	for (size_t i = 0; i < l.length(); ++i) {
		auto ch = l[i];

		switch (ch)
		{
		case L'"':
			++i;
			for (; ; ++i) {
				if (i == l.length())
					throw invalid_argument("ToDo"); //ToDo
				ch = l[i];
				if (ch == L'"')
					break;
				*last << ch;
			}
			break;
		case L':':
			if (last == &arg)
				last = &val; else
				*last << ch;
			break;
		case L' ':
			p(arg.str(), val.str());
			arg.str(L"");
			val.str(L"");
			last = &arg;
			break;
		default:
			*last << ch;
			break;
		}
		
	}

	if (arg.str().length()) p(arg.str(), val.str());
}

bool SortElements(bool first) {
	if (!first) return false;
	wcout << L"Cтрока сортировки: ";

	LList<std::function<int(ProductData&, ProductData&)>> sorters{};
	UseWords(Console::ReadLine(), [&sorters](wstring&& arg, wstring&& val) {
		if (val.length() != 1) throw invalid_argument("ToDo"); //ToDo

		if (arg == L"name"s)
			switch (val[0]) {
			case L'>':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d1.get_name().compare(d2.get_name());
				} });
				break;
			case L'<':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d2.get_name().compare(d1.get_name());
				} });
				break;
			default:
				throw invalid_argument("ToDo"); //ToDo
			}
		else if (arg == L"procucer"s)
			switch (val[0]) {
			case L'>':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d1.get_producer().compare(d2.get_producer());
				} });
				break;
			case L'<':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d2.get_producer().compare(d1.get_producer());
				} });
				break;
			default:
				throw invalid_argument("ToDo"); //ToDo
			}
		else if (arg == L"part count"s)
			switch (val[0]) {
			case L'>':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d1.get_part_count() - d2.get_part_count();
				} });
				break;
			case L'<':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d2.get_part_count() - d2.get_part_count();
				} });
				break;
			default:
				throw invalid_argument("ToDo"); //ToDo
			}
		else if (arg == L"cost"s)
			switch (val[0]) {
			case L'>':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d1.get_cost_in_dollars() - d2.get_cost_in_dollars();
				} });
				break;
			case L'<':
				sorters.Add({ [](ProductData& d1, ProductData& d2) {
					return d2.get_cost_in_dollars() - d1.get_cost_in_dollars();
				} });
				break;
			default:
				throw invalid_argument("ToDo"); //ToDo
			}
		else
			throw invalid_argument("ToDo"); //ToDo

	});

	sub_l.Sort([&sorters](ProductData& d1, ProductData& d2) {
		for (auto& f : sorters)
		{
			auto res = f(d1, d2);
			if (res) return res;
		}
		return 0;
	});

	return true;
}

bool FindElements(bool first) {
	if (!first) return true;
	wcout << L"Cтрока поиска: ";

	UseWords(Console::ReadLine(), [] (wstring&& arg, wstring&& val) {
		if (val.length()) {
			auto comp = val[0];

			if (comp == L'<')
			{
				val = val.substr(1);
				sub_l.RemoveWhere([&arg, &val](ProductData& d) {
					if (arg == L"name"s)
						return !(d.get_name() < val);
					if (arg == L"producer"s)
						return !(d.get_producer() < val);
					try {
						auto val_i = stoi(val);
						if (arg == L"part count"s)
							return !(d.get_part_count() < val_i);
						else if (arg == L"cost"s)
							return !(d.get_cost_in_dollars() < val_i);
					}
					catch (invalid_argument e) {}

					return true;
				});
			}
			else if (comp == L'>')
			{
				val = val.substr(1);
				sub_l.RemoveWhere([&arg, &val](ProductData& d) {
					if (arg == L"name"s)
						return !(d.get_name() > val);
					if (arg == L"producer"s)
						return !(d.get_producer() > val);
					try {
						auto val_i = stoi(val);
						if (arg == L"part count"s)
							return !(d.get_part_count() > val_i);
						else if (arg == L"cost"s)
							return !(d.get_cost_in_dollars() > val_i);
					}
					catch (invalid_argument e) {}

					return true;
				});
			}
			else {
				sub_l.RemoveWhere([&arg, &val](ProductData& d) {
					if (arg == L"name"s)
						return !(d.get_name() == val);
					if (arg == L"producer"s)
						return !(d.get_producer() == val);
					try {
						auto val_i = stoi(val);
						if (arg == L"part count"s)
							return !(d.get_part_count() == val_i);
						else if (arg == L"cost"s)
							return !(d.get_cost_in_dollars() == val_i);
					}
					catch (invalid_argument e) {}

					return true;
				});
			}

		}
		else {
			sub_l.RemoveWhere([&arg](ProductData& d) {
				if (d.get_name() == arg) return false;
				if (d.get_producer() == arg) return false;

				try {
					auto arg_i = stoi(arg);
					if (d.get_part_count() == arg_i) return false;
					if (d.get_cost_in_dollars() == arg_i) return false;
				}
				catch (invalid_argument e) {}

				return true;
			});
		}
	});

	return true;
}

bool RemoveFoundElements(bool) {
	for (auto& d : sub_l)
		l.Remove(d);
	sub_l.Clear();
	return false;
}

int main()
{
	if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
	if (_setmode(_fileno(stdin),  _O_U16TEXT) == -1) return -1;
	if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

	//l << ProductData{ L"abc", L"def", 123, 456 };

	auto m = Menu{l, L"Главное меню",
		Menu{l, L"Добавить элементы",
			Menu{l, L"Ввести с клавиатуры", ReadElements },
			Menu{l, L"Сгенерировать случайно", GenerateElements },
			Menu{l, L"Загрузить из файла", LoadElements },
		},
		Menu{sub_l, L"Использовать элементы", CopyList,
			Menu{sub_l, L"Сохранить в файл", SaveElements },
			Menu{sub_l, L"Отсортировать", SortElements },
			Menu{sub_l, L"Найти", FindElements,
				Menu{sub_l, L"Удалить", RemoveFoundElements }
			},
		},
	};

	m.Show();
}