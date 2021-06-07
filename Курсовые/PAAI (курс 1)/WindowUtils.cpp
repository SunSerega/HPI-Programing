#include "WindowUtils.h"

#include <Windows.h>

void Halt(int code)
{
	PostQuitMessage(code);
}