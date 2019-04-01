#include "RenderEngine.h"
#include <gl/glew.h>
#include "../MyGlWindow.h"
#include "../Logger.h"
#include "fstream"

MyGlWindow* window;

RenderEngine::RenderEngine() {}

GLuint RenderEngine::depthProgramID;
uniLocFuncPtr RenderEngine::callback;

void RenderEngine::DrawAllVertexBuffers(float* cameraPos, float* cameraViewDir)
{
	for (int j = 0; j < BufferManager::m_numVertexBuffers; ++j)
	{
		BufferManager::m_bufferPool[j].m_List.drawNodes(cameraPos, cameraViewDir);
	}
}

void RenderEngine::DrawAllVertexBuffersDepth()
{
	for (int j = 0; j < BufferManager::m_numVertexBuffers; ++j)
	{
		BufferManager::m_bufferPool[j].m_List.drawNodes(depthProgramID, callback);
	}
}

void RenderEngine::Initialize(MyGlWindow* _window)
{
	window = _window;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//Alpha
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Shadows
	//glDepthMask(GL_FALSE);
	//glDepthFunc(GL_LESS);

	BufferManager::Initialize();
}

void RenderEngine::Shutdown()
{
	
}

void RenderEngine::AddGeometry(void* vertices, Geometry& mesh)
{
	vertices;
	mesh;
}

void RenderEngine::AddGeometry(void* vertices, void* indices, Geometry& mesh)
{
	BufferManager::AddGeometry(vertices, indices, mesh);
}

void RenderEngine::AddRenderable(Renderable* info)
{
	BufferManager::AddRenderInfo(info);
}

void RenderEngine::Draw(float* cameraPos, float* cameraViewDir)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, window->width(), window->height());
	DrawAllVertexBuffers(cameraPos, cameraViewDir);
}

void RenderEngine::DepthDraw(const GLuint SHADOW_WIDTH, const GLuint SHADOW_HEIGHT)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	DrawAllVertexBuffersDepth();
}

bool RenderEngine::CheckStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		printf(buffer);
		std::string message = "";
		message += "Shader Failed to Load";
		LOG(Error, message.c_str());
		delete[] buffer;
		exit(1);
	}
	return true;
}

bool RenderEngine::CheckShaderStatus(GLuint shaderID)//checks to make sure shader compiled
{
	return CheckStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool RenderEngine::CheckProgramStatus(GLuint _programID)//checks to make sure shader linked
{
	return CheckStatus(_programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

bool RenderEngine::ValidateStatus(GLuint _programID)
{
	GLint status;
	glValidateProgram(_programID);
	glGetProgramiv(_programID, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE)
	{
		//std::cout << "Program Invalid" << std::endl;
		LOG(Error, "Program Invalid");
		return false;
	}
	//std::cout << "Program Valid" << std::endl;
	LOG(Info, "Program Valid");
	return true;
}

std::string RenderEngine::ReadShaderCode(const char* fileName)
{
	std::ifstream myInput(fileName);
	if (!myInput.good())
	{
		std::string message = "";
		message += "Failed to read Shader";
		LOG(Error, message.c_str());
		exit(1);
	}
	return std::string(std::istreambuf_iterator<char>(myInput), std::istreambuf_iterator<char>());
}

void RenderEngine::InstallShaders(GLuint* programID, const char* vertexShader, const char* fragmentShader)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	std::string temp = ReadShaderCode(vertexShader);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode(fragmentShader);
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	CheckShaderStatus(vertexShaderID);
	CheckShaderStatus(fragmentShaderID);

	*programID = glCreateProgram();
	glAttachShader(*programID, vertexShaderID);
	glAttachShader(*programID, fragmentShaderID);
	glLinkProgram(*programID);
	
	CheckProgramStatus(*programID);
	ValidateStatus(*programID);
}

void RenderEngine::InstallShaders(GLuint* programID, const char* vertexShader, const char* geometryShader, const char* fragmentShader)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	std::string temp = ReadShaderCode(vertexShader);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode(geometryShader);
	adapter[0] = temp.c_str();
	glShaderSource(geometryShaderID, 1, adapter, 0);
	temp = ReadShaderCode(fragmentShader);
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(geometryShaderID);
	glCompileShader(fragmentShaderID);

	CheckShaderStatus(vertexShaderID);
	CheckShaderStatus(geometryShaderID);
	CheckShaderStatus(fragmentShaderID);

	*programID = glCreateProgram();
	glAttachShader(*programID, vertexShaderID);
	glAttachShader(*programID, geometryShaderID);
	glAttachShader(*programID, fragmentShaderID);
	glLinkProgram(*programID);

	CheckProgramStatus(*programID);
	ValidateStatus(*programID);
}