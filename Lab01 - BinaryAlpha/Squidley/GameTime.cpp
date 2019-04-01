#include "GameTime.h"

bool gameTime::Initialize()
{
	if (!QueryPerformanceFrequency(&timeFrequency))
	{
		return false;
	}
	if (!QueryPerformanceCounter(&timeLastFrame))
	{
		return false;
	}
	return true;
}

bool gameTime::Shutdown()
{
	return true;
}

void gameTime::newFrame()
{
	LARGE_INTEGER thisTime;
	QueryPerformanceCounter(&thisTime);
	LARGE_INTEGER delta;
	delta.QuadPart = thisTime.QuadPart - timeLastFrame.QuadPart;
	deltaTime = ((float)delta.QuadPart) / timeFrequency.QuadPart;
	timeLastFrame.QuadPart = thisTime.QuadPart;
}

float gameTime::timeElapsedLastFrame() const
{
	return deltaTime;
}
