#include "SteeringBehaviors.h"
#include "..\Random.h"

#define WANDER_LO -1.0f
#define WANDER_HI 1.0f

float SteeringBehaviors::rotX;
float SteeringBehaviors::rotY;
float SteeringBehaviors::rotZ;

glm::vec3 SteeringBehaviors::wanderTarget;

glm::vec3 SteeringBehaviors::seek(glm::vec3 pos, glm::vec3 target, glm::vec3 velocity, float maxSpeed, float maxRotate)
{
	if (target == pos) return glm::vec3();
	glm::vec3 desiredVelocity = glm::normalize(target - pos) * maxSpeed;
	glm::vec3 steering = desiredVelocity - velocity;
	if (glm::length(steering) > maxRotate)
		steering = glm::normalize(steering) * maxRotate;

	return steering;
}

glm::vec3 SteeringBehaviors::flee(glm::vec3 pos, glm::vec3 target, glm::vec3 velocity, float maxSpeed, float maxRotate)
{
	return -seek(pos, target, velocity, maxSpeed, maxRotate);
}

glm::vec3 SteeringBehaviors::pursuit(glm::vec3 pos, glm::vec3 targetPos, glm::vec3 targetVel, glm::vec3 velocity, float maxSpeed, float maxRotate)
{
	return seek(pos, targetPos + targetVel, velocity, maxSpeed, maxRotate);
}

glm::vec3 SteeringBehaviors::evade(glm::vec3 pos, glm::vec3 targetPos, glm::vec3 targetVel, glm::vec3 velocity, float maxSpeed, float maxRotate)
{
	return flee(pos, targetPos + targetVel, velocity, maxSpeed, maxRotate);
}

glm::vec3 SteeringBehaviors::separation(glm::vec3* flockerPos, int numFlockers, glm::vec3 curPos)
{
	glm::vec3 totPos;

	for (int j = 0; j < numFlockers; ++j)
	{
		totPos += flockerPos[j] - curPos;
	}

	totPos /= numFlockers;

	totPos *= -1;

	if (totPos.x != 0.0f || totPos.y != 0.0f || totPos.z != 0.0f)
		totPos = glm::normalize(totPos);

	return totPos;
}

glm::vec3 SteeringBehaviors::cohesion(glm::vec3* flockerPos, int numFlockers, glm::vec3 curPos)
{
	glm::vec3 totPos;

	for (int j = 0; j < numFlockers; ++j)
	{
		totPos += flockerPos[j];
	}

	glm::vec3 avgPos;
	avgPos = totPos / (float)numFlockers;
	avgPos -= curPos;
	if (avgPos.x != 0.0f || avgPos.y != 0.0f || avgPos.z != 0.0f)
		avgPos = glm::normalize(avgPos);

	return avgPos;
}

glm::vec3 SteeringBehaviors::alignment(glm::vec3* flockerVel, int numFlockers, glm::vec3 vel)
{
	glm::vec3 totVel;
	

	for (int j = 0; j < numFlockers; ++j)
	{
		totVel += flockerVel[j];
	}

	glm::vec3 avgVel;
	avgVel = totVel / (float)numFlockers;
	avgVel -= vel;
	if (avgVel.x != 0.0f || avgVel.y != 0.0f || avgVel.z != 0.0f)
		avgVel = glm::normalize(avgVel);

	return avgVel;
}

glm::vec3 SteeringBehaviors::flocking(glm::vec3* flockerPos, glm::vec3* flockerVel, int numFlockers, glm::vec3 curPos, glm::vec3 vel, 
	float maxSpeed, float maxRotate, float alignmentScale, float coshesionScale, float separationScale)
{
	maxSpeed;
	glm::vec3 totVel;

	glm::vec3 tmpPos[75];
	glm::vec3 tmpVel[75];

	int neighborCount = 0;

	for (int j = 0; j < numFlockers; ++j)
	{
		if (flockerPos[j] != curPos)
		{
			if (glm::distance(curPos, flockerPos[j]) < 7.5f)
			{
				tmpPos[neighborCount] = flockerPos[j];
				tmpVel[neighborCount] = flockerVel[j];
				++neighborCount;
			}
		}
	}

	if (neighborCount == 0)
	{
		return glm::normalize(vel);// *maxSpeed;
	}

	glm::vec3 alignmentVec = alignment(tmpVel, neighborCount, vel);
	glm::vec3 cohesionVec = cohesion(tmpPos, neighborCount, curPos);
	glm::vec3 separationVec = separation(tmpPos, neighborCount, curPos);

	totVel = alignmentVec * alignmentScale + cohesionVec * coshesionScale + separationVec * separationScale;

	if (totVel.x != 0.0f || totVel.y != 0.0f || totVel.z != 0.0f)
		totVel = glm::normalize(totVel);// *maxSpeed;

	if (glm::length(totVel) > maxRotate)
		return glm::normalize(totVel) * maxRotate;

	return totVel;// *0.2f;
}

glm::vec3 SteeringBehaviors::arrival(glm::vec3 pos, glm::vec3 target, glm::vec3 velocity, float maxSpeed, float maxRotate, float slowingDistance)
{
	glm::vec3 targetOffset = target - pos;
	float distance = glm::length(targetOffset);
	if (distance > slowingDistance)
		return seek(pos, target, velocity, maxSpeed, maxRotate);
	float rampedSpeed = maxSpeed * (distance / slowingDistance);
	float clippedSpeed = glm::min(rampedSpeed, maxSpeed);
	glm::vec3 desiredVelocity = (clippedSpeed / distance) * targetOffset;
	glm::vec3 steering = desiredVelocity - velocity;
	if (steering.length() > maxRotate)
		steering = glm::normalize(steering) * maxRotate;

	return steering;
}

glm::vec3 SteeringBehaviors::interpose(glm::vec3 pos, glm::vec3 target1Pos, glm::vec3 target1Vel, glm::vec3 target2Pos, glm::vec3 target2Vel, glm::vec3 velocity, float maxSpeed, float maxRotate)
{
	if (target1Pos == target2Pos) return glm::vec3();
	//glm::vec3 halfway = glm::normalize(target1 - target2) * (float)(glm::vec3(target1 - target2).length() / 2);
	glm::vec3 halfway = glm::vec3(target1Pos + target1Vel - target2Pos + target2Vel) / 2.0f;
	halfway = halfway + target2Pos;// +target1Vel;
	return seek(pos, halfway, velocity, maxSpeed, maxRotate);
}

glm::vec3 SteeringBehaviors::wander(glm::vec3 pos, glm::vec3 vel, float circleDist, float circleRad)
{
	/*glm::vec3 dist = glm::normalize(vel) * circleDist;
	rotX += WANDER_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (WANDER_HI - WANDER_LO)));
	rotY += WANDER_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (WANDER_HI - WANDER_LO)));
	rotZ += WANDER_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (WANDER_HI - WANDER_LO)));
	if (rotX >= 360) rotX -= 360;
	if (rotX <= 0) rotX += 360;
	if (rotY >= 360) rotY -= 360;
	if (rotY <= 0) rotY += 360;
	if (rotZ >= 360) rotZ -= 360;
	if (rotZ <= 0) rotZ += 360;
	glm::mat4 dirMat = glm::rotate(glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	dirMat *= glm::rotate(glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	dirMat *= glm::rotate(glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
	
	glm::vec3 newDir = glm::vec3(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) * dirMat) * circleRad;
	glm::vec3 tmp = vel - dist + newDir;
	return vel - dist + newDir;*/

	wanderTarget += glm::vec3(Random::randomInRange(WANDER_LO, WANDER_HI), Random::randomInRange(WANDER_LO, WANDER_HI), Random::randomInRange(WANDER_LO, WANDER_HI));
	wanderTarget = glm::normalize(wanderTarget) * circleRad;
	//target *= circleRad;
	//printf("%f, %f, %f\n", target.x, target.y, target.z);
	glm::vec3 dist = glm::normalize(vel) * circleDist;
	//printf("%f, %f, %f\n", dist.x, dist.y, dist.z);
	glm::vec3 localTarget = dist + wanderTarget;
	//printf("%f, %f, %f\n", localTarget.x, localTarget.y, localTarget.z);
	return localTarget + pos - vel;
}
