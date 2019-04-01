#pragma once
#include "../../Renderable.h"
#include "../Component.h"
#include "SpatialComponent.h"

class RenderableComponent : public Component
{
public:
	Renderable* getRenderable();
	void setRenderable(Renderable*);

	void makeRendered();
	void makeNotRendered();

	bool isRendered();

	bool Update(float) override;

private:
	Renderable* mesh;
	SpatialComponent* space;
};

