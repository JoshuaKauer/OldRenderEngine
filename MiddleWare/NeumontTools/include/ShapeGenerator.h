#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H
#include "glm\glm.hpp"
#include "TypeDefs.h"
#include "ShapeData.h"
#include "ExportImportHeader.h"

#include <Tools\System\Random.h>

namespace Neumont
{
	// After you call any function in this class, you are responsible to delete
	// all the data referenced by the returned ShapeData. You can call
	// ShapeData::cleanUp() to do this for you if you like.
	class DLL_SHARED ShapeGenerator
	{
	public:
		// Makes a line from (-1, 0, 0) to (1, 0, 0)
		// Must render with GL_LINES
		static ShapeData makeLine();
		// Render GL_TRIANGLES
		static ShapeData makePlane(uint dimensions);
		// Render GL_LINES
		static ShapeData makeWireframePlane(uint dimensions);
		static ShapeData makeCube();
		static ShapeData makeCubeWithTangents();		//Added by YYF; utilizes code from makeCube(), but replaces Color with Tangent. Code is in Rendering/Geometries/ShadeDataCubeWithTangent
		static ShapeData makeSphere(uint tesselation);
		static ShapeData makeTorus(uint tesselation);
		static ShapeData makeArrow();
		static ShapeData makeTeapot(uint tesselation, const glm::mat4& lidTransform);
		// Generates a ShapeData for GL_LINES that will render 
		// white normals for the given ShapeData. Only the Vertex 
		// positions and colors will have any meaning
		static ShapeData generateNormals(const ShapeData& data);
	private:
		static void makeTeapot(
			unsigned int grid, const glm::mat4& lidTransform,
			unsigned int& numVerts,
			float*& verts,
			float*& normals, 
			float*& textureCoordinates,
			unsigned short*& indices, 
			unsigned int& numIndices);

		static ShapeData makePlaneVerts(uint tesselation);
		static ShapeData makePlaneIndices(uint tesselation);
		static ShapeData makePlaneUnseamedIndices(uint tesselation);

		// Teapot:
		static void generatePatches(float * v, float * n, float *tc, unsigned short* el, int grid);
		static void buildPatchReflect(int patchNum,
			float *B, float *dB,
			float *v, float *n, float *, unsigned short *el,
			int &index, int &elIndex, int &, int grid,
			bool reflectX, bool reflectY);
		static void buildPatch(glm::vec3 patch[][4],
			float *B, float *dB,
			float *v, float *n,float *, unsigned short *el,
			int &index, int &elIndex, int &, int grid, glm::mat3 reflect, bool invertNormal);
		static void getPatch( int patchNum, glm::vec3 patch[][4], bool reverseV );
		static void computeBasisFunctions( float * B, float * dB, int grid );
		static glm::vec3 evaluate( int gridU, int gridV, float *B, glm::vec3 patch[][4] );
		static glm::vec3 evaluateNormal( int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4] );
		static void moveLid(int,float *,glm::mat4);

	public:
		static ShapeData makeCone(uint tesselation)
		{
			// Cone head
			vec3 coneTip(/*CONE_HEIGHT*/ 0.5f, 0.0f, 0.0f);
			float coneRadius = 0.2f;

			// Rotate thisVert around x axis
			glm::vec4 thisVert(0, coneRadius, 0, 0);
			glm::mat4 rotator = glm::rotate(glm::mat4(), 360.0f / tesselation, glm::vec3(1.0f, 0.0f, 0.0f));

			ShapeData ret;
			// 2 verts at same position around the rim so I can vary the normals
			// + 2 -> one for cone tip and one for cone base center
			ret.numVerts = 2 * tesselation + 2; 
			ret.verts = new Vertex[ret.numVerts];

			uint coneTipIndex = tesselation * 2;
			uint coneBaseCenterIndex = coneTipIndex + 1;
			for(uint i = 0; i < tesselation; i++)
			{
				// Side facing triangle vert
				Vertex& v = ret.verts[i];
				v.color = glm::vec4(RandomColor(), 1);
				v.position = vec3(thisVert);
				// Just want its position in the YZ plane, ignore x value;
				v.normal = glm::normalize(glm::vec3(0, v.position.y, v.position.z));
				thisVert = rotator * thisVert;
			}

			for(uint i = tesselation; i < tesselation * 2; i++)
			{
				// Cone bottom facing vert. Everything is same except the normal
				Vertex& v = ret.verts[i - tesselation];
				Vertex& v2 = ret.verts[i];
				v2.color = v.color;
				v2.position = v.position;
				v2.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
			}

			ret.verts[coneTipIndex].position = coneTip;
			ret.verts[coneTipIndex].color = glm::vec4(RandomColor(), 1);
			ret.verts[coneTipIndex].normal = glm::vec3(1.0f, 0.0f, 0.0f);

			ret.verts[coneBaseCenterIndex].position = glm::vec3(0, 0.0f, 0.0f);
			ret.verts[coneBaseCenterIndex].color = glm::vec4(RandomColor(), 1);
			ret.verts[coneBaseCenterIndex].normal = glm::vec3(-1.0f, 0.0f, 0.0f);


			const uint NUM_VERTS_PER_TRI = 3;
			ret.numIndices = NUM_VERTS_PER_TRI * tesselation * 2;
			ret.indices = new ushort[ret.numIndices];

			uint indiceIndex = 0;
			for(uint i = 0; i < tesselation; i++)
			{
				// Side face
				ret.indices[indiceIndex++] = i;
				ret.indices[indiceIndex++] = (i + 1) % tesselation;
				ret.indices[indiceIndex++] = coneTipIndex;
			}
			for(uint i = tesselation; i < tesselation * 2; i++)
			{
				// Bottom face
				ret.indices[indiceIndex++] = i;
				ret.indices[indiceIndex++] = (i + 1) % tesselation;
				ret.indices[indiceIndex++] = coneBaseCenterIndex;
			}
			assert(indiceIndex == ret.numIndices);

			// Cynlindar stem
			return ret;
		}

		static ShapeData makeCylinder(uint tesselation)
		{
			ShapeData ret;
			ret.numVerts = tesselation * 2 + 2; // + 2 for top and bottom center
			ret.verts = new Vertex[ret.numVerts];

			glm::vec4 thisVert(0, 1, 0, 0);
			glm::mat4 rotator = glm::rotate(glm::mat4(), 360.0f / tesselation, glm::vec3(1.0f, 0.0f, 0.0f));

			for(uint i = 0; i < tesselation; i++)
			{
				Vertex& v0 = ret.verts[i];
				Vertex& v1 = ret.verts[i + tesselation];

				v0.position = vec3(thisVert);
				v0.position.x = 1.0f;
				v1.position = vec3(thisVert);

				v0.color = glm::vec4(RandomColor(), 1);
				v1.color = glm::vec4(RandomColor(), 1);
				v0.normal.x = +1.0f;
				v1.normal.x = -1.0f;

				thisVert = rotator * thisVert;
			}
			uint topCenterVertIndex = ret.numVerts - 2;
			uint bottomCenterVertIndex = ret.numVerts - 1;
			ret.verts[topCenterVertIndex].position.x = 1.0f;
			ret.verts[topCenterVertIndex].normal.x = +1.0f;
			ret.verts[topCenterVertIndex].color = glm::vec4(RandomColor(), 1);
			ret.verts[bottomCenterVertIndex].normal.x = -1.0f;
			ret.verts[bottomCenterVertIndex].color = glm::vec4(RandomColor(), 1);

			const uint NUM_TRIS_PER_SIDE_FACE = 2;
			const uint NUM_TRIS_PER_END_FACE = 1;
			const uint NUM_ENDS = 2;
			const uint NUM_VERTS_PER_TRI = 3;
			const uint NUM_SIDE_INDICES = tesselation * NUM_TRIS_PER_SIDE_FACE * NUM_VERTS_PER_TRI;
			const uint NUM_END_INDICES = tesselation * NUM_TRIS_PER_END_FACE * NUM_VERTS_PER_TRI * NUM_ENDS;
			ret.numIndices = NUM_SIDE_INDICES + NUM_END_INDICES;
			ret.indices = new ushort[ret.numIndices];

			uint indiceIndex = 0;
			for(uint i = 0; i < tesselation; i++)
			{
				// Top
				ret.indices[indiceIndex++] = i;
				ret.indices[indiceIndex++] = (i + 1) % tesselation;
				ret.indices[indiceIndex++] = topCenterVertIndex;
			} 
			for(uint i = 0; i < tesselation; i++)
			{
				// Bottom
				ret.indices[indiceIndex++] = i + tesselation;
				ret.indices[indiceIndex++] = (i + 1 < tesselation) ? i + 1 + tesselation : tesselation;
				ret.indices[indiceIndex++] = bottomCenterVertIndex;
			}

			// Side
			for(uint i = 0; i < tesselation; i++)
			{
				// Face 1
				ret.indices[indiceIndex++] = i;
				ret.indices[indiceIndex++] = (i + 1) % tesselation;
				ret.indices[indiceIndex++] = i + tesselation;

				// Face 2
				ret.indices[indiceIndex++] = (i + 1) % tesselation;
				ret.indices[indiceIndex++] = (i + 1 < tesselation) ? (i + 1 + tesselation) : i + 1;
				ret.indices[indiceIndex++] = i + tesselation;
			}
			assert(indiceIndex == ret.numIndices);
			return ret;
		}
	};
}
#endif