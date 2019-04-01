#pragma once
#include "Geometry.h"
#include "TransformInfo.h"
#include "VertexShaderInfo.h"
#include "FragmentShaderInfo.h"
#include "TextureInfo.h"
#include "gl/glew.h"
class Renderable
{
	
public:
	static bool useGeometry;
	static bool useLightPass;
	static bool drawLightCube;
	bool drawWithFrameBuffer = true;
	bool geometryPass = false;
	bool lightingPass = false;
	bool lightCube = false;
	Geometry* m_mesh;
	TransformInfo m_transformInfo;
	FragmentShaderInfo* m_fragmentShaderInfo;
	TextureInfo m_textureInfo;
	VertexShaderInfo m_vertexShaderInfo;
	bool m_isRendered = true;
	void setMesh(Geometry*);
	Geometry* getMesh();
	void setTransformInfo(const TransformInfo&);
	TransformInfo getTransformInfo();
	void setVertexShaderInfo(VertexShaderInfo);
	VertexShaderInfo getVertexShaderInfo();
	void setFragmentShaderInfo(FragmentShaderInfo*);
	FragmentShaderInfo* getFragmentShaderInfo();
	void setTextureInfo(TextureInfo);
	TextureInfo getTextureInfo();
	void Renderable::DepthTest(GLuint programID, void(*uniLocFuncPtr)(Renderable*, float* cameraPos, float* cameraViewDir), float* cameraPos = 0, float* cameraViewDir = 0);
	void Draw(float* cameraPos = 0, float* cameraViewDir = 0);

};

