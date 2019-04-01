#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "Renderable.h"

class Ray
{
public:
	static float rayTriangleIntersect(
		const glm::vec3 &rayOrg,	//origin of the ray
		const glm::vec3 &rayDelta,	//ray length and direction
		const glm::vec3 &p0,		//traingle vertices
		const glm::vec3 &p1,		//traingle vertices
		const glm::vec3 &p2,		//traingle vertices
		float minT,					//closest intersection found, start with far plane more recommended //Tutorial says start at 1.0 though
		glm::vec3 &offset			//offset to move the cube by
		);

	static float fireRay(Renderable* collideRenderables, int renderCount, glm::vec3 pos, glm::vec3 direction);
};
