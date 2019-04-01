#include "KeyboardComponent.h"
#include "../../../ObjConverter/ConfigReader.h"
#include <Windows.h>
#include "MovementComponent.h"
#include "RenderableComponent.h"

bool KeyboardComponent::Initialize()
{
	move = GetSiblingComponent<MovementComponent>();
	rend = GetSiblingComponent<RenderableComponent>();
	return true;
}

bool KeyboardComponent::Update(float dt)
{
	if (GetAsyncKeyState('W'))
	{
		move->moveForward(dt);
	}
	if (GetAsyncKeyState('S'))
	{
		move->moveBackward(dt);
	}
	if (GetAsyncKeyState('A'))
	{
		move->strafeLeft(dt);
	}
	if (GetAsyncKeyState('D'))
	{
		move->strafeRight(dt);
	}
	if (GetAsyncKeyState('R'))
	{
		move->moveUp(dt);
	}
	if (GetAsyncKeyState('F'))
	{
		move->moveDown(dt);
	}
	if (GetAsyncKeyState('J'))
	{
		if (rend->isRendered()) rend->makeNotRendered();
		else rend->makeRendered();
	}
	return true;
}

bool KeyboardComponent::Draw()
{

	return true;
}