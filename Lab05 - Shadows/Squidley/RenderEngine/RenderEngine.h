#pragma once
#include "..\\Geometry.h"
#include "BufferManager.h"
#include <string>

class MyGlWindow;

class RenderEngine
{
	RenderEngine();
	static void DrawAllVertexBuffers();
	static void DrawAllVertexBuffersDepth();
public:
	static GLuint depthProgramID;
	static uniLocFuncPtr callback;

	static void Initialize(MyGlWindow* window);
	static void Shutdown();
	static void AddGeometry(void* vertices, Geometry& mesh);
	static void AddGeometry(void* vertices, void* indices, Geometry& mesh);
	static void AddRenderable(Renderable* info);
	static void Draw();
	static void DepthDraw(const GLuint SHADOW_WIDTH, const GLuint SHADOW_HEIGHT);
	static bool CheckStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType);
	static bool CheckShaderStatus(GLuint shaderID);
	static bool CheckProgramStatus(GLuint _programID);
	static bool ValidateStatus(GLuint _programID);
	static std::string ReadShaderCode(const char* fileName);
	static void InstallShaders(GLuint* programID, const char* vertexShader, const char* fragmentShader);
	static void InstallShaders(GLuint* programID, const char* vertexShader, const char* geometryShader, const char* fragmentShader);
};