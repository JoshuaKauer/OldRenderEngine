#ifndef DECAL_H
#define DECAL_H

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm\gtx\transform.hpp>
#pragma warning(pop)

struct Triangle
{
	unsigned short	index[3];
};

struct ColorRGBA
{
	float		red;
	float		green;
	float		blue;
	float		alpha;

	ColorRGBA() {}

	ColorRGBA(float r, float g, float b, float a)
	{
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}
};

class Decal
{
private:
	const static float decalEpsilon;

	glm::vec3	decalCenter;
	glm::vec3	decalNormal;

	glm::vec4	leftPlane;
	glm::vec4	rightPlane;
	glm::vec4	bottomPlane;
	glm::vec4	topPlane;
	glm::vec4	frontPlane;
	glm::vec4	backPlane;

	const static long maxDecalVertices = 256;

	long		decalVertexCount;
	long		decalTriangleCount;

	glm::vec3	vertexArray[maxDecalVertices];
	glm::vec2	texcoordArray[maxDecalVertices];
	ColorRGBA	colorArray[maxDecalVertices];
	Triangle	triangleArray[maxDecalVertices];

	bool AddPolygon(long vertexCount, const glm::vec3* vertex, const glm::vec3* normal);
	void ClipMesh(long triangleCount, const Triangle* triangle, const glm::vec3 *vertex, const glm::vec3 *normal);
	long ClipPolygon(long vertexCount, const glm::vec3* vertex, const glm::vec3* normal, glm::vec3* newVertex, glm::vec3* newNormal) const;
	static long ClipPolygonAgainstPlane(const glm::vec4& plane, long vertexCount, const glm::vec3* vertex, const glm::vec3* normal, glm::vec3* newVertex, glm::vec3* newNormal);

public:

	Decal(const glm::vec3& center, const glm::vec3& normal, const glm::vec3& tangent, float width, float height, float depth);
};

#endif