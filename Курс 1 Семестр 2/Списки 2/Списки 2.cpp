#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "LList.h"
#include "ProductData.h"
#include "Console.h"

using namespace std;

auto l = LList<ProductData>();

void Header() {
	Console::Clear();
	wcout << l;
}

#pragma region Menu

class Menu {
private:
	wstring name;
	bool (*proc)() = nullptr;
	LList<Menu> sub_menus{};

	inline void AddSubMenus() {}
	template <class T0>
	inline void AddSubMenus(T0&& el0) {
		//ToDo here and else - forward instead of move
		sub_menus << move(el0);
	}
	template <class T0, class ... Ts>
	inline void AddSubMenus(T0&& el0, Ts&&... sub_menus) {
		this->sub_menus << move(el0);
		AddSubMenus(move(sub_menus)...);
	}

public:

	template <class ... Ts>
	Menu(wstring name, Ts&&... sub_menus)
		: name(name)
	{
		AddSubMenus(move(sub_menus)...);
	}

	template <class ... Ts>
	Menu(wstring name, bool (*proc)(), Ts&&... sub_menus)
		: name(name), proc(proc)
	{
		AddSubMenus(move(sub_menus)...);
	}

	Menu(Menu&& m) noexcept
		: name(move(m.name))
		, proc(m.proc)
		, sub_menus(move(m.sub_menus))
	{}

	void Show() {
		while (true) {
			Console::SetTitle(name);
			if (proc && !proc()) return;

			if (sub_menus.Count()) {
				Header();

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
				if (choice_is_int && !choice_i) return;

				auto invalid_choice = true;
				if (choice_is_int && (unsigned)(choice_i - 1) < c)
				{
					auto itr = sub_menus.begin();
					while (--choice_i) ++itr;
					(*itr).Show();
					invalid_choice = false;
				}
				else for (auto& m : sub_menus)
				{
					if (m.name == choice)
					{
						m.Show();
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

#pragma endregion Menu

bool ReadElements() {
	while (true) {
		Header();

		ProductData pd;
		if (wcin >> pd)
			l << pd; else
		{
			wcin.clear();
			return false;
		}

	}
}

bool GenerateElements() {
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

bool LoadElements() {
	auto fname = Console::Read<wstring>(L"Имя файла (откуда загружать): ");
	auto br = BinaryReader(fname);

	auto c = br.Read7bit<int>();
	while (c--)
		l << br.Read<ProductData>();

	return false;
}

bool SaveElements() {
	auto fname = Console::Read<wstring>(L"Имя файла (куда сохранять): ");
	auto bw = BinaryWriter(fname);

	bw.Write7bit(l.Count());
	for (auto& pd : l)
		bw << pd;

	return false;
}

int main()
{
	if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) return -1;
	if (_setmode(_fileno(stdin),  _O_U16TEXT) == -1) return -1;
	if (_setmode(_fileno(stderr), _O_U16TEXT) == -1) return -1;

	//l << ProductData{ L"abc", L"def", 123, 456 };

	auto m = Menu{ L"Главное меню",
		Menu{ L"Добавить элементы",
			Menu{ L"Ввести с клавиатуры", ReadElements },
			Menu{ L"Сгенерировать случайно", GenerateElements },
			Menu{ L"Загрузить из файла", LoadElements },
		},
		Menu{ L"Использовать элементы",
			Menu{ L"Сохранить в файл", SaveElements },
			Menu{ L"Отсортировать" },
			Menu{ L"Найти",
				Menu{ L"Удалить" }
			},
		},
	};

	m.Show();
}