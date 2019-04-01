#include "MyGame.h"
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4251)
#include <QtGui/qmouseevent>
#pragma warning(pop)
#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm\gtx\transform.hpp>
#pragma warning(pop)
#include "GameTime.h"
#include "ShapeGenerator.h"
#include "Reader.h"
#include "MyGlWindow.h"
#include "MyModel.h"
#include "Logger.h"
#include "RenderEngine/RenderEngine.h"
#include "ECS/Components/KeyboardComponent.h"
#include "ECS/Components/MouseComponent.h"
#include "ECS/Components/CameraComponent.h"
#include "ECS/Components/MovementComponent.h"
#include "ECS/Components/SpatialComponent.h"
#include "ECS/Components/RenderableComponent.h"
#include "ECS/Entities/PlayerEntity.h"
#include <ctime>

#define FarPlane 500.0f
#define MAX_RENDERABLES 500

bool spacePressed = false;
bool pauseTime = false;
bool pressed = false;

float dt = 1/60;
float totalTime = 0.0f;
float waterTime = 0.0f;
GLfloat height_scale = 0.1f;

GLuint depthMapFBO;
GLuint depthCubemap;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
glm::mat4 shadowTransforms[6];

GLuint refractMapFBO;
GLuint refractCubeMap;
GLuint depthrenderbuffer;
GLuint REFRACT_WIDTH = SCREEN_HEIGHT;
GLuint REFRACT_HEIGHT = SCREEN_HEIGHT;
glm::mat4 refractTransforms[6];

GLuint gBuffer, gPosition, gNormal, gAlbedoSpec, rboDepth;

Reader read;

gameTime gTime;

CameraComponent camera;
MouseComponent mouse;
KeyboardComponent keys;
MovementComponent move;
SpatialComponent space, aSpace;
RenderableComponent rend, aRend;

MyGlWindow* m_window;

Geometry* cube;

Renderable	render[MAX_RENDERABLES];

short numObjects = 9;
glm::vec3 jainaPositions[] = 
{
	glm::vec3(-3.0, -3.0, -3.0),
	glm::vec3(0.0, -3.0, -3.0),
	glm::vec3(3.0, -3.0, -3.0),
	glm::vec3(-3.0, -3.0, 0.0),
	glm::vec3(0.0, -3.0, 0.0),
	glm::vec3(3.0, -3.0, 0.0),
	glm::vec3(-3.0, -3.0, 3.0),
	glm::vec3(0.0, -3.0, 3.0),
	glm::vec3(3.0, -3.0, 3.0),
};

const GLuint NR_LIGHTS = 32;
glm::vec3 lightPositions[NR_LIGHTS];
glm::vec3 lightColors[NR_LIGHTS];

unsigned short renderCount = 2;

MyModel model;

PlayerEntity player;

int nodeCount = 0;

GLuint passThroughProgramID, pTShaderProgramID, pCshaderProgramID, pCTShaderProgramID, 
		pCNShaderProgramID, pTAlphaShaderProgramID, explodeShaderProgramID, waterShaderProgramID, 
		skyboxShaderProgramID, environmentRefactionShaderProgramID, shadowShaderProgramID, 
		depthShaderProgramID, pTNShaderProgramID, normalMappingShaderProgramID, parallaxMappingShaderProgramID,
		geometryPassProgramID, lightingPassProgramID, lightCubeProgramID;

glm::vec3 cubeTrans;

glm::vec3 lightPosition = glm::vec3(-3.0f, 3.0f, -2.0f);

glm::vec3 stealthPos = glm::vec3(0.0f, 20.0f, 0.0f);

glm::mat4 modelToProjectionMatrix;
glm::mat4 worldToProjectionMatrix;

void uniformCallback(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackExplode(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackLight(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackWater(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackSkybox(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackRefraction(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackDepth(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackShadow(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackBetterLighting(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackNormalMapping(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackParallaxMapping(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackGeometryPass(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackLightingPass(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);
void uniformCallbackLightCubePass(Renderable*, float* cameraPos = 0, float* cameraViewDir = 0);

void MyGame::PrintFrames()
{
	frameCount += 1;
	accDt += dt;

	if (accDt >= 5.0f)
	{
		printf("%f\n", (frameCount / accDt));
		frameCount = 0;
		accDt = 0.0f;
	}
}

MyGame::MyGame(){}

bool MyGame::Initialize(MyGlWindow* window)
{
	m_window = window;
	RenderEngine::Initialize(window);

	srand(static_cast <unsigned> (time(nullptr)));

	RenderEngine::InstallShaders(&pTShaderProgramID, "..\\Data\\Shaders\\PTVertexShader.glsl", "..\\Data\\Shaders\\PTFragmentShader.glsl");
	RenderEngine::InstallShaders(&passThroughProgramID, "..\\Data\\Shaders\\VertexShaderPassThru.glsl", "..\\Data\\Shaders\\FragmentShaderPassThru.glsl");
	RenderEngine::InstallShaders(&pCshaderProgramID, "..\\Data\\Shaders\\PCVertexShader.glsl", "..\\Data\\Shaders\\PCFragmentShader.glsl");
	RenderEngine::InstallShaders(&pCTShaderProgramID, "..\\Data\\Shaders\\PCTVertexShader.glsl", "..\\Data\\Shaders\\PCTFragmentShader.glsl");
	RenderEngine::InstallShaders(&pCNShaderProgramID, "..\\Data\\Shaders\\PCNVertexShader.glsl", "..\\Data\\Shaders\\PCNFragmentShader.glsl");
	RenderEngine::InstallShaders(&pTAlphaShaderProgramID, "..\\Data\\Shaders\\PTAlphaVertexShader.glsl", "..\\Data\\Shaders\\PTAlphaFragmentShader.glsl");
	RenderEngine::InstallShaders(&explodeShaderProgramID, "..\\Data\\Shaders\\ExplodeVertexShader.glsl", "..\\Data\\Shaders\\ExplodeGeometryShader.glsl", "..\\Data\\Shaders\\ExplodeFragmentShader.glsl");
	RenderEngine::InstallShaders(&waterShaderProgramID, "..\\Data\\Shaders\\WaterVertexShader.glsl", "..\\Data\\Shaders\\WaterFragmentShader.glsl");
	RenderEngine::InstallShaders(&skyboxShaderProgramID, "..\\Data\\Shaders\\SkyboxVertexShader.glsl", "..\\Data\\Shaders\\SkyboxFragmentShader.glsl");
	RenderEngine::InstallShaders(&environmentRefactionShaderProgramID, "..\\Data\\Shaders\\EnvironmentRefractionVertex.glsl", "..\\Data\\Shaders\\EnvironmentRefractionFragment.glsl");
	RenderEngine::InstallShaders(&depthShaderProgramID, "..\\Data\\Shaders\\DepthVertexShader.glsl", "..\\Data\\Shaders\\DepthGeometryShader.glsl", "..\\Data\\Shaders\\DepthFragmentShader.glsl");
	RenderEngine::InstallShaders(&shadowShaderProgramID, "..\\Data\\Shaders\\ShadowMappingVertex.glsl", "..\\Data\\Shaders\\ShadowMappingFragment.glsl");
	RenderEngine::InstallShaders(&pTNShaderProgramID, "..\\Data\\Shaders\\PTNVertexShader.glsl", "..\\Data\\Shaders\\PTNFragmentShader.glsl");
	RenderEngine::InstallShaders(&normalMappingShaderProgramID, "..\\Data\\Shaders\\NormalMappingVertexShader.glsl", "..\\Data\\Shaders\\NormalMappingFragmentShader.glsl");
	RenderEngine::InstallShaders(&parallaxMappingShaderProgramID, "..\\Data\\Shaders\\ParallaxMappingVertexShader.glsl", "..\\Data\\Shaders\\ParallaxMappingFragmentShader.glsl");
	RenderEngine::InstallShaders(&geometryPassProgramID, "..\\Data\\Shaders\\GeometryPassVertexShader.glsl", "..\\Data\\Shaders\\GeometryPassFragmentShader.glsl");
	RenderEngine::InstallShaders(&lightingPassProgramID, "..\\Data\\Shaders\\LightingPassVertexShader.glsl", "..\\Data\\Shaders\\LightingPassFragmentShader.glsl");
	RenderEngine::InstallShaders(&lightCubeProgramID, "..\\Data\\Shaders\\LightCubeVertexShader.glsl", "..\\Data\\Shaders\\LightCubeFragmentShader.glsl");

	RenderEngine::depthProgramID = depthShaderProgramID;
	RenderEngine::callback = uniformCallbackDepth;

	for (GLuint j = 0; j < NR_LIGHTS; ++j)
	{
		// Calculate slightly random offsets
		GLfloat xPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
		GLfloat yPos = ((rand() % 100) / 100.0f) * 6.0f - 4.0f;
		GLfloat zPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
		lightPositions[j] = glm::vec3(xPos, yPos, zPos);
		// Also calculate random color
		GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5f; // Between 0.5 and 1.0
		GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5f; // Between 0.5 and 1.0
		GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5f; // Between 0.5 and 1.0
		lightColors[j] = glm::vec3(rColor, gColor, bColor);
	}

	SendDataToOpenGL();

	player.AddComponent(&space, "Spatial");
	player.AddComponent(&move, "Move");
	player.AddComponent(&keys, "Keys");
	player.AddComponent(&mouse, "Mouse");
	player.AddComponent(&camera, "Camera");
	player.Init();

	gTime.Initialize();

	//glGenFramebuffers(1, &depthMapFBO);
	//// Create depth cubemap texture
	//glGenTextures(1, &depthCubemap);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	//for (GLuint i = 0; i < 6; ++i)
	//{
	//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//}
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//// Attach cubemap as depth map FBO's color buffer
	//glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	LOG(Error, "Framebuffer not complete!"); 
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//glGenTextures(1, &refractCubeMap);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, refractCubeMap);
	//for (GLuint j = 0; j < 6; ++j)
	//{
	//	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGB, REFRACT_WIDTH, REFRACT_HEIGHT, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);
	//}

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenFramebuffers(1, &refractMapFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, refractMapFBO);

	////Optional? depth buffer
	//glGenRenderbuffers(1, &depthrenderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, REFRACT_WIDTH, REFRACT_HEIGHT);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	////Attach it
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, refractMapFBO);
	//
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, refractCubeMap, 0);

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	LOG(Error, "Framebuffer not complete!");
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


	glUseProgram(lightingPassProgramID);
	glUniform1i(glGetUniformLocation(lightingPassProgramID, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(lightingPassProgramID, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightingPassProgramID, "gAlbedoSpec"), 2);

	
	// Set up G-Buffer
	// 3 textures:
	// 1. Positions (RGB)
	// 2. Color (RGB) + Specular (A)
	// 3. Normals (RGB) 
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_window->width(), m_window->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_window->width(), m_window->height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// - Color + Specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_window->width(), m_window->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);
	// - Create and attach depth buffer (renderbuffer)
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_window->width(), m_window->height());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	LOG(Info, "Game Initialized");
	return true;
}

bool MyGame::Shutdown()
{
	glUseProgram(0);
	m_window->closeProgram();
	return true;
}

void MyGame::Update()
{
	gTime.newFrame();
	Draw();
	dt = gTime.timeElapsedLastFrame();
	if(!pauseTime)
		totalTime += dt;
	waterTime += dt;
	player.Update(dt);
	ProcessKeys();
	PrintFrames();
}

void MyGame::ProcessKeys()
{
	if (GetAsyncKeyState('X'))
	{
		Shutdown();
	}
	if (GetAsyncKeyState(VK_SPACE) && !spacePressed)
	{
		pauseTime = !pauseTime;
		spacePressed = true;
	}
	else if (!GetAsyncKeyState(VK_SPACE))
	{
		spacePressed = false;
	}
}

void MyGame::ProcessMouse(QMouseEvent* e)
{
	if (e->buttons() & Qt::RightButton)
	{
		mouse.mouseUpdate(glm::vec2(e->x(), e->y()));
	}
}

// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

// RenderCube() Renders a 1x1 3D cube in NDC.
GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
void RenderCube()
{
	// Initialize (if necessary)
	if (cubeVAO == 0)
	{
		GLfloat vertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
															  // Front face
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
															   // Left face
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
															  // Right face
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
															 // Bottom face
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
																// Top face
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// Render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void MyGame::Draw()
{
	// 1. Geometry Pass: render scene's geometry/color data into gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Renderable::useGeometry = true;
	Renderable::useLightPass = false;
	Renderable::drawLightCube = false;
	RenderEngine::Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glm::mat4 view = camera.getWorldToViewMatrix();

	// 2. Lighting Pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(lightingPassProgramID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	for (GLuint i = 0; i < NR_LIGHTS; i++)
	{
		glUniform3fv(glGetUniformLocation(lightingPassProgramID, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
		glUniform3fv(glGetUniformLocation(lightingPassProgramID, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
		// Update attenuation parameters and calculate radius
		const GLfloat constant = 1.0f; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const GLfloat linear = 0.7f;
		const GLfloat quadratic = 1.8f;
		glUniform1f(glGetUniformLocation(lightingPassProgramID, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(lightingPassProgramID, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
		// Then calculate radius of light volume/sphere
		const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
		GLfloat radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2 * quadratic);
		glUniform1f(glGetUniformLocation(lightingPassProgramID, ("lights[" + std::to_string(i) + "].Radius").c_str()), radius);
	}
	glUniform3fv(glGetUniformLocation(lightingPassProgramID, "viewPos"), 1, &camera.position[0]);
	RenderQuad();

	// 2.5. Copy content of geometry's depth buffer to default framebuffer's depth buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
	// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
	// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
	// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
	glBlitFramebuffer(0, 0, m_window->width(), m_window->height(), 0, 0, m_window->width(), m_window->height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 3. Render lights on top of scene, by blitting
	glUseProgram(lightCubeProgramID);
	glUniformMatrix4fv(glGetUniformLocation(lightCubeProgramID, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(lightCubeProgramID, "view"), 1, GL_FALSE, &view[0][0]);
	glm::mat4 model;
	for (GLuint i = 0; i < NR_LIGHTS; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, lightPositions[i]);
		model = glm::scale(model, glm::vec3(0.25f));
		glUniformMatrix4fv(glGetUniformLocation(lightCubeProgramID, "model"), 1, GL_FALSE, &model[0][0]);
		glUniform3fv(glGetUniformLocation(lightCubeProgramID, "lightColor"), 1, &lightColors[i][0]);
		RenderCube();
	}
}

void MyGame::SendDataToOpenGL()
{
	TextureInfo texture;
	TransformInfo pos;
	TransformInfo scale;
	cube = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.pt.scene");

	Geometry* jaina = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\jaina.scene");

	//Geometry* scene = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Plane.pt.scene");

	//Geometry* skybox = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.pt.scene");

	for (int j = 0; j < numObjects; ++j)
	{
		render[j].setMesh(jaina);
		render[j].m_vertexShaderInfo.programID = geometryPassProgramID;
		render[j].m_vertexShaderInfo.callback = uniformCallbackGeometryPass;
		pos = render[j].getTransformInfo();
		pos.m_translateTransform = jainaPositions[j];
		pos.m_scaleTransform = glm::vec3(0.75f, 0.75f, 0.75f);
		render[j].setTransformInfo(pos);
		texture = render[j].getTextureInfo();
		texture.loadBMP_custom("..\\Data\\Textures\\jaina_diffuse.bmp");
		texture.loadNormalMap("..\\Data\\Textures\\jaina_normal.bmp");
		texture.loadParallaxMap("..\\Data\\Textures\\jaina_specular.bmp");
		render[j].setTextureInfo(texture);
		render[j].geometryPass = true;

		RenderEngine::AddRenderable(&render[j]);
	}

	/*for (GLuint j = numObjects; j < NR_LIGHTS + numObjects; ++j)
	{
		render[j].setMesh(skybox);
		render[j].m_vertexShaderInfo.programID = lightCubeProgramID;
		render[j].m_vertexShaderInfo.callback = uniformCallbackLightCubePass;
		render[j].lightingPass = true;
		scale = render[j].getTransformInfo();
		scale.m_scaleTransform = glm::vec3(0.25f, 0.25f, 0.25f);
		render[j].setTransformInfo(scale);

		RenderEngine::AddRenderable(&render[j]);
	}

	render[NR_LIGHTS + numObjects].setMesh(scene);
	render[NR_LIGHTS + numObjects].m_vertexShaderInfo.programID = lightingPassProgramID;
	render[NR_LIGHTS + numObjects].m_vertexShaderInfo.callback = uniformCallbackLightingPass;
	render[NR_LIGHTS + numObjects].lightCube = true;

	RenderEngine::AddRenderable(&render[NR_LIGHTS + numObjects]);*/
}

MyModel* MyGame::GetModel()
{
	return &model;
}

void MyGame::glGetViewProjectionUniformLocation(Renderable* randy, float* projection, float* cameraView)
{
	if (projection)
	{
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, projection);
	}
	else
	{
		glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, &viewToProjectionMatrix[0][0]);
	}

	if (cameraView)
	{
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, cameraView);
	}
	else
	{
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, &camera.getWorldToViewMatrix()[0][0]);
	}
}

void uniformCallback(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);
	
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "myTexture"), 0);
}

void uniformCallbackExplode(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, &camera.getWorldToViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->getTransformInfo().getFinalTransform()[0][0]);
	glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "time"), totalTime);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "texture_diffuse1"), 0);
}

void uniformCallbackLight(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);

	GLuint ambientLightUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "ambientLight");
	GLuint diffuseLightUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightPositionWorld");
	GLuint eyePositionUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "eyePositionWorld");
	GLuint specularExpUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "specularExp");

	glm::vec4 ambientLight = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	glm::vec3 lightPositionWorld = lightPosition;
	glm::vec3 eyePosition = camera.getPosition();
	float specularExp = 500.0f;

	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	glUniform3fv(diffuseLightUniformLocation, 1, &lightPositionWorld[0]);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	glUniform1f(specularExpUniformLocation, specularExp);

	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);
}

void uniformCallbackWater(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);

	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);

	glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "time"), waterTime);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "myTexture"), 0);
}

void uniformCallbackSkybox(Renderable* randy, float* projection, float* cameraView)
{
	glm::mat4 view;

	if (projection)
	{
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, projection);
	}
	else
	{
		glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, &viewToProjectionMatrix[0][0]);
	}

	if (cameraView)
	{
		view[0][0] = cameraView[0];
		view[0][1] = cameraView[1];
		view[0][2] = cameraView[2];
		view[0][3] = cameraView[3];
		view[1][0] = cameraView[4];
		view[1][1] = cameraView[5];
		view[1][2] = cameraView[6];
		view[1][3] = cameraView[7];
		view[2][0] = cameraView[8];
		view[2][1] = cameraView[9];
		view[2][2] = cameraView[10];
		view[2][3] = cameraView[11];
		view[3][0] = cameraView[12];
		view[3][1] = cameraView[13];
		view[3][2] = cameraView[14];
		view[3][3] = cameraView[15];

		view = glm::mat4(glm::mat3(view));
	}
	else
	{
		view = glm::mat4(glm::mat3(camera.getWorldToViewMatrix()));
	}
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "skybox"), 0);
}

void uniformCallbackRefraction(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);

	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "cameraPos"), 1, &camera.position[0]);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "environment"), 0);
}

void uniformCallbackDepth(Renderable* randy, float* projection, float* cameraView)
{
	projection;
	cameraView;
	glUniformMatrix4fv(glGetUniformLocation(depthShaderProgramID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);
	for (GLuint i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(glGetUniformLocation(depthShaderProgramID, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, &shadowTransforms[i][0][0]);
	}
	glUniform1f(glGetUniformLocation(depthShaderProgramID, "far_plane"), FarPlane);
	glUniform3fv(glGetUniformLocation(depthShaderProgramID, "lightPos"), 1, &lightPosition[0]);
}

void uniformCallbackShadow(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, 1.0f, FarPlane);

	shadowTransforms[0] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms[1] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms[2] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms[3] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms[4] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms[5] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);
	// Set light uniforms
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightPos"), 1, &lightPosition[0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "viewPos"), 1, &camera.position[0]);
	
	glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "far_plane"), FarPlane);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "depthMap"), 1);
}

void uniformCallbackBetterLighting(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);
	// Set light uniforms
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightPos"), 1, &lightPosition[0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "viewPos"), 1, &camera.position[0]);


	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "floorTexture"), 0);
}

void uniformCallbackNormalMapping(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);

	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->getTransformInfo().getFinalTransform()[0][0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightPos"), 1, &lightPosition[0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "viewPos"), 1, &camera.position[0]);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "diffuseMap"), 0);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "normalMap"), 1);
}

void uniformCallbackParallaxMapping(Renderable*randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);

	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->getTransformInfo().getFinalTransform()[0][0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightPos"), 1, &lightPosition[0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "viewPos"), 1, &camera.position[0]);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "diffuseMap"), 0);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "normalMap"), 1);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "depthMap"), 2);

	glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "height_scale"), height_scale);
}

void uniformCallbackGeometryPass(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);

	for (int j = 0; j < numObjects; j++)
	{
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->getTransformInfo().getFinalTransform()[0][0]);
	}

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "texture_diffuse1"), 0);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "texture_specular1"), 2);
}

void uniformCallbackLightingPass(Renderable* randy, float* projection, float* cameraView)
{
	projection;
	cameraView;
	for (GLuint j = 0; j < NR_LIGHTS; j++)
	{
		glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, ("lights[" + std::to_string(j) + "].Position").c_str()), 1, &lightPositions[j][0]);
		glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, ("lights[" + std::to_string(j) + "].Color").c_str()), 1, &lightColors[j][0]);
		// Update attenuation parameters and calculate radius
		const GLfloat constant = 1.0f; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const GLfloat linear = 0.7f;
		const GLfloat quadratic = 1.8f;
		glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, ("lights[" + std::to_string(j) + "].Linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, ("lights[" + std::to_string(j) + "].Quadratic").c_str()), quadratic);
	}
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "viewPos"), 1, &camera.position[0]);
}

void uniformCallbackLightCubePass(Renderable* randy, float* projection, float* cameraView)
{
	MyGame::glGetViewProjectionUniformLocation(randy, projection, cameraView);
	for (GLuint j = 0; j < NR_LIGHTS; j++)
	{
		glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->getTransformInfo().getFinalTransform()[0][0]);
		glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightColor"), 1, &lightColors[j][0]);
	}

}