#include "Ray.h"
#include "VertexFormats.h"
#include <stdio.h>
#include "Logger.h"

float Ray::rayTriangleIntersect(const glm::vec3 &rayOrg, const glm::vec3 &rayDelta, const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2, float minT, glm::vec3 &offset)
{
	//We'll return this huge number if no intersection is detected
	const float noIntersection = FLT_MAX;

	//Return distance of far plane to because don't want
	//to place it past that anyways and using it for comparisons
	//const float noIntersection = FarPlane;

	//Compute clockwise edge vectors
	glm::vec3 e1 = p1 - p0;
	glm::vec3 e2 = p2 - p1;

	//Compute surface normal (Unnormalized)
	glm::vec3 n = glm::cross(e1, e2);

	//Compute gradient, which tells us how steep of an angle
	//we are approaching the *front* side of the triangle
	float dot = glm::dot(n, rayDelta);

	//Check for a ray that is parallel to the triangle, or not
	//pointin towards the front face of the triangle
	//
	//Note that this also will reject degenerate triangles and
	//rays as well.  We code this in a very particular
	//way so that NANs will bail here.  (i.e. this does NOT
	//behave the same as "dot >= 0.0f" when NANs are involved)
	if (!(dot < 0.0f))
		return noIntersection;

	//Compute d value for the plane equation.  We will
	//use the plane equation with d on the right side:
	//Ax + By + Cz = d
	float d = glm::dot(n, p0);

	//Compute parametric point of intersection with the plane
	//containing the triangle, checking at the earliest
	//possible stages for trivial rejection
	float t = d - glm::dot(n, rayOrg);

	//is ray origin on the backside of the polygon?  Again,
	//we phrase the check so that NANs will bail
	if (!(t <= 0.0f))
		return noIntersection;

	//Closer intersection already found? (Or does
	//ray not reach the plane?)
	//
	//since dot < 0
	//
	//		t / dot > minT
	//
	//is the same as
	//
	//		t < dot * minT
	//
	//(And then we invert it for NAN checking...)
	if (!(t >= dot*minT))
		return noIntersection;

	//OK, ray intersects the plane.  Compute actual parametric
	//point of intersection
	t /= dot;
	//assert(t >= 0.0f);
	LOG(Warning, "Ray Triangle Intersect >= 0");
	//assert(t <= minT);
	LOG(Warning, "Ray Triangle Intersect <= minT");

	//Compute 3D point of intersection
	glm::vec3 p = rayOrg + rayDelta * t;

	//Find dominant axis to selcect which plane
	//to project onto, and compute u's and v's
	float u0, u1, u2;
	float v0, v1, v2;
	if (fabs(n.x) > fabs(n.y))
	{
		if (fabs(n.x) > fabs(n.z))
		{
			u0 = p.y - p0.y;
			u1 = p1.y - p0.y;
			u2 = p2.y - p0.y;

			v0 = p.z - p0.z;
			v1 = p1.z - p0.z;
			v2 = p2.z - p0.z;
		}
		else
		{
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.y - p0.y;
			v1 = p1.y - p0.y;
			v2 = p2.y - p0.y;
		}
	}
	else
	{
		if (fabs(n.y) > fabs(n.z))
		{
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.z - p0.z;
			v1 = p1.z - p0.z;
			v2 = p2.z - p0.z;
		}
		else
		{
			u0 = p.x - p0.x;
			u1 = p1.x - p0.x;
			u2 = p2.x - p0.x;

			v0 = p.y - p0.y;
			v1 = p1.y - p0.y;
			v2 = p2.y - p0.y;
		}
	}

	// Compute denominator, check for invalid
	float temp = u1 * v2 - v1 * u2;
	if (!(temp != 0.0f))
		return noIntersection;

	temp = 1.0f / temp;

	//Compute barycentric coords, checking for out-of-range
	//at each step
	float alpha = (u0 * v2 - v0 * u2) * temp;
	if (!(alpha >= 0.0f))
		return noIntersection;

	float beta = (u1 * v0 - v1 * u0) * temp;
	if (!(beta >= 0.0f))
		return noIntersection;

	float gamma = 1.0f - alpha - beta;
	if (!(gamma >= 0.0f))
		return noIntersection;

	//Normalize n to make the proper offset
	offset = glm::normalize(n);
	//Return parametric point of intersection
	return t;
}

float Ray::fireRay(Renderable* collideRenderables, int renderCount, glm::vec3 pos, glm::vec3 direction)
{
	glm::vec3 pos1;
	glm::vec3 pos2;
	glm::vec3 pos3;

	glm::vec3 offset;

	float distance2Object = 200.0f;

	for (int i = 0; i < renderCount; ++i)
	{
		switch (collideRenderables[i].m_mesh->vertexFormat)
		{
		case PositionOnly:
		{
			Position *vertices = reinterpret_cast <Position*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{
				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		case PositionColor:
		{
			PColor *vertices = reinterpret_cast <PColor*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{
				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		case PositionTexture:
		{
			PText *vertices = reinterpret_cast <PText*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{
				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		case PositionNormal:
		{
			PNorm *vertices = reinterpret_cast <PNorm*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{
				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		case PositionColorNormal:
		{
			PCNorm *vertices = reinterpret_cast <PCNorm*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{
				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		case PositionTextureNormal:
		{
			PTNorm *vertices = reinterpret_cast <PTNorm*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{
				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		case PositionColorTexture:
		{
			PCText *vertices = reinterpret_cast <PCText*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{

				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		case PositionColorTextureNormal:
		{
			PCTN *vertices = reinterpret_cast <PCTN*> (collideRenderables[i].m_mesh->vertices);
			for (unsigned int j = 0; j < collideRenderables[i].m_mesh->m_vertexCount;)
			{
				pos1 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos2 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				pos3 = glm::vec3(collideRenderables[i].m_transformInfo.getFinalTransform() * glm::vec4(vertices[j++].position, 1.0f));
				float temp = Ray::rayTriangleIntersect(pos, direction, pos1, pos2, pos3, distance2Object, offset);
				if (temp < distance2Object)
					distance2Object = temp;
			}
			break;
		}
		default:
			printf("Positions, Colors, Textures, Normals aren't being defined.\nVertex Format : %i\nMade it all the way to fireRay\n", collideRenderables[i].m_mesh->vertexFormat);
			exit(1);
			break;
		}
	}
	//if (distance2Object < FarPlane)
	//{
	//	cubeTrans = camera.viewDirection * distance2Object + camera.position + offset;
	//	Boop();
	//}

	return distance2Object;
}