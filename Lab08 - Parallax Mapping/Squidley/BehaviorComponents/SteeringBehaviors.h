#pragma once
#pragma warning(push)
#pragma warning(disable : 4201)
#pragma warning(disable : 4458)
#include <glm/glm.hpp>
#pragma warning(pop)
#include "..\ECS\Component.h"

class SteeringBehaviors : public Component
{
private:
	static float rotX, rotY, rotZ;
	static glm::vec3 wanderTarget;
public:
	virtual bool Update(float dt) override { return dt == dt; };

	glm::vec3 seek(glm::vec3, glm::vec3, glm::vec3, float, float);

	glm::vec3 flee(glm::vec3, glm::vec3, glm::vec3, float, float);

	glm::vec3 pursuit(glm::vec3, glm::vec3, glm::vec3, glm::vec3, float, float);

	glm::vec3 evade(glm::vec3, glm::vec3, glm::vec3, glm::vec3, float, float);

	glm::vec3 separation(glm::vec3* flockerPos, int numFlockers, glm::vec3 curPos);
	
	glm::vec3 cohesion(glm::vec3* flockerPos, int numFlockers, glm::vec3 curPos);
	
	glm::vec3 alignment(glm::vec3* flockerVel, int numFlockers, glm::vec3 curPos);
	
	glm::vec3 flocking(glm::vec3* flockerPos, glm::vec3* flockerVel, int numFlockers, glm::vec3 curPos, glm::vec3 vel, float maxSpeed, float maxRotate, float alignmentScale, float coshesionScale, float separationScale);

	glm::vec3 arrival(glm::vec3, glm::vec3, glm::vec3, float, float, float);

	glm::vec3 interpose(glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, glm::vec3, float, float);

	glm::vec3 wander(glm::vec3, glm::vec3, float, float);
};