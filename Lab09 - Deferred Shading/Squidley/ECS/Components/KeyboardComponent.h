#pragma once
#include "../Component.h"

class MovementComponent;
class RenderableComponent;
class KeyboardComponent : public Component
{
private:
	MovementComponent* move;
	RenderableComponent* rend;

public:
	bool Initialize() override;
	bool Update(float dt) override;
	bool Draw() override;
	
};

