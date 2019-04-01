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

void Renderable::DepthTest(GLuint programID, void(*uniLocFuncPtr)(Renderable*, float* cameraPos, float* cameraViewDir), float* cameraPos, float* cameraViewDir)
{
	if (m_isRendered)
	{
		glUseProgram(programID);

		glBindVertexArray(m_mesh->m_VAOID);

		uniLocFuncPtr(this, cameraPos, cameraViewDir);

		glDrawElementsBaseVertex(m_mesh->glEnums, m_mesh->m_indexCount, GL_UNSIGNED_INT, (void*)m_mesh->m_indexByteOffset, m_mesh->m_vertexByteOffset / m_mesh->m_vertexStride);
	}
}

void Renderable::Draw(float* cameraPos, float* cameraViewDir)
{
	if (cameraPos && cameraViewDir)
	{
		if (!drawWithFrameBuffer)
		{
			return;
		}
	}
	if (m_isRendered)
	{
		if (!this->m_textureInfo.isCubeMap)
		{
			glEnable(GL_CULL_FACE);
			glDepthFunc(GL_LESS);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureInfo.textureID);
		}
		else
		{
			glDisable(GL_CULL_FACE);
			glDepthFunc(GL_LEQUAL);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureInfo.textureID);
		}

		if (m_textureInfo.shadowTextureID)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, *m_textureInfo.shadowTextureID);
		}

		if (m_textureInfo.normalMapID)
		{
			if (m_textureInfo.shadowTextureID)
			{
				glActiveTexture(GL_TEXTURE2);
			}
			else
			{
				glActiveTexture(GL_TEXTURE1);
			}
			glBindTexture(GL_TEXTURE_2D, m_textureInfo.normalMapID);
		}

		if (m_textureInfo.parallaxMapID)
		{
			if (m_textureInfo.shadowTextureID)
			{
				glActiveTexture(GL_TEXTURE3);
			}
			else
			{
				glActiveTexture(GL_TEXTURE2);
			}
			glBindTexture(GL_TEXTURE_2D, m_textureInfo.parallaxMapID);
		}

		glUseProgram(m_vertexShaderInfo.programID);

		glBindVertexArray(m_mesh->m_VAOID);

		if (m_vertexShaderInfo.callback)
			m_vertexShaderInfo.callback(this, cameraPos, cameraViewDir);

		glDrawElementsBaseVertex(m_mesh->glEnums, m_mesh->m_indexCount, GL_UNSIGNED_INT, (void*)m_mesh->m_indexByteOffset, m_mesh->m_vertexByteOffset / m_mesh->m_vertexStride);
	}
}