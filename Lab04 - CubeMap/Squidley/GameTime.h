#ifndef GAMETIME_H
#define GAMETIME_H
#include <Windows.h>

class gameTime
{
private:
	LARGE_INTEGER timeFrequency;
	LARGE_INTEGER timeLastFrame;
	LARGE_INTEGER deltaLastFrame;
	float deltaTime;
public:
	bool Initialize();
	bool Shutdown();
	void newFrame();
	float timeElapsedLastFrame() const;
};

#endif