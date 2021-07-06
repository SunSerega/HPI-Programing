#include "OpenGLUtils.h"



void ThrowIfErr()
{
	auto err = glGetError();
	if (err) throw err;
}


