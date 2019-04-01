#include "Renderable.h"
#include <gl/glew.h>
#include "Vertex.h"

void Renderable::setMesh(Geometry* mesh)
{
	m_mesh = mesh;
}
Geometry* Renderable::getMesh()
{
	return m_mesh;
}
void Renderable::setTransformInfo(const TransformInfo& transformInfo)
{
	m_transformInfo = transformInfo;
}
TransformInfo Renderable::getTransformInfo()
{
	return m_transformInfo;
}
void Renderable::setVertexShaderInfo(VertexShaderInfo vertexShaderInfo)
{
	m_vertexShaderInfo = vertexShaderInfo;
}
VertexShaderInfo Renderable::getVertexShaderInfo()
{
	return m_vertexShaderInfo;
}
void Renderable::setFragmentShaderInfo(FragmentShaderInfo* fragmentShaderInfo)
{
	m_fragmentShaderInfo = fragmentShaderInfo;
}
FragmentShaderInfo* Renderable::getFragmentShaderInfo()
{
	return m_fragmentShaderInfo;
}
void Renderable::setTextureInfo(TextureInfo textureInfo)
{
	m_textureInfo = textureInfo;
}
TextureInfo Renderable::getTextureInfo()
{
	return m_textureInfo;
}

void Renderable::Draw()
{
	if (m_isRendered)
	{
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureInfo.textureID);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureInfo.shadowTextureID);

		glUseProgram(m_vertexShaderInfo.programID);

		glBindVertexArray(m_mesh->m_VAOID);

		if (m_vertexShaderInfo.callback)
			m_vertexShaderInfo.callback(this);

		glDrawElementsBaseVertex(m_mesh->glEnums, m_mesh->m_indexCount, GL_UNSIGNED_INT, (void*)m_mesh->m_indexByteOffset, m_mesh->m_vertexByteOffset / m_mesh->m_vertexStride);
	}
}