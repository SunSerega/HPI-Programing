#pragma once

#include <iostream>
#include <string>

namespace Console {

	void Clear(wchar_t fill = L' ');

	void SetTitle(std::wstring title);

	std::wstring ReadLine();
	void IgnoreLine();

	template<class T>
	T Read() {
		T res;
		std::wcin >> res;
		IgnoreLine();
		return res;
	}
	template<class T>
	T Read(std::wstring prompt) {
		std::wcout << prompt;
		T res;
		std::wcin >> res;
		IgnoreLine();
		return res;
	}

}
