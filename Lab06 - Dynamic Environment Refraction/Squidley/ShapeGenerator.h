#pragma once
#include "ShapeData.h"
#include "Geometry.h"

#define MAX_GEOMETRIES 300

typedef struct{ glm::vec3 p1, p2, p3; } Facet3;

class ShapeGenerator
{
	static unsigned numGeo;
	static Geometry geo[MAX_GEOMETRIES];
	static ShapeData makePlaneVerts(GLuint dimensions);
	static ShapeData makePlaneIndices(GLuint dimensions);
	static int generateSphere(Facet3* facets, int iterations);
	static void buildSphere(glm::vec3 color);
public:
	static Geometry* makeScene(char*);
	static Geometry* makeLine(glm::vec3, glm::vec3);

	static ShapeData makeTriangle();
	static ShapeData makeCube();
	static Geometry* makeCube2();
	static ShapeData makeCube(glm::vec3 color);
	static ShapeData makeDebugCube(glm::vec3 color);
	static ShapeData makeLine(glm::vec3 point1, glm::vec3 point2, glm::vec3 color);
	static ShapeData makeYGrid(glm::vec3 color, int range);
	static ShapeData makePlane(glm::vec3 upperLeft, glm::vec3 lowerRight);
	static ShapeData makePlane(GLuint dimensions = 10);
	static ShapeData makePyramid();
	static Geometry* makePyramid2();
	static ShapeData makeSphere(glm::vec3 color);
	static ShapeData generateNormals(const ShapeData& data);
};

