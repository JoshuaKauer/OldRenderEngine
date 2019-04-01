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
	char* name;

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
	void Renderable::DepthTest(GLuint programID, void(*uniLocFuncPtr)(Renderable*));
	void Draw();

};

