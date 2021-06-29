#include "WindowUtils.h"

#include <Windows.h>

void Halt(int code)
{
	PostQuitMessage(code);
}

Event::Event(bool is_manual, bool state)
	: ev{ CreateEvent(nullptr, is_manual, state, nullptr) }
{};
void Event::Set() {
	if (!SetEvent(ev)) throw 0;
}
void Event::Reset() {
	if (!ResetEvent(ev)) throw 0;
}
void Event::Wait() {
	if (auto res = WaitForSingleObject(ev, INFINITE)) throw res;
}

