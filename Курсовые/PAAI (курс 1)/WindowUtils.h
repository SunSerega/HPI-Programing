#pragma once



void Halt(int code = 0);

class Event {
	void* ev;

public:
	Event(bool is_manual, bool state);

	void Set();
	void Reset();
	void Wait();

};


