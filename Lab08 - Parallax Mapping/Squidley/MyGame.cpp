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

unsigned short renderCount = 2;

MyModel model;

PlayerEntity player;

int nodeCount = 0;

GLuint passThroughProgramID, pTShaderProgramID, pCshaderProgramID, pCTShaderProgramID, 
		pCNShaderProgramID, pTAlphaShaderProgramID, explodeShaderProgramID, waterShaderProgramID, 
		skyboxShaderProgramID, environmentRefactionShaderProgramID, shadowShaderProgramID, 
		depthShaderProgramID, pTNShaderProgramID, normalMappingShaderProgramID, parallaxMappingShaderProgramID;

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

	RenderEngine::depthProgramID = depthShaderProgramID;
	RenderEngine::callback = uniformCallbackDepth;

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

void MyGame::Draw()
{
	RenderEngine::Draw();
}

void MyGame::SendDataToOpenGL()
{
	TextureInfo texture;
	TransformInfo pos;
	TransformInfo scale;
	cube = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.pt.scene");

	Geometry* scene = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Plane.ptntb.scene");

	Geometry* clone = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Clonetrooper.scene");

	Geometry* water = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Water.scene");

	Geometry* skybox = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.p.scene");

	Geometry* stealth = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Clonetrooper.pn.scene");

	//Hideout
	render[0].setMesh(scene);
	render[0].m_vertexShaderInfo.programID = parallaxMappingShaderProgramID;
	render[0].m_vertexShaderInfo.callback = uniformCallbackParallaxMapping;
	texture = render[0].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\wood.bmp");
	texture.loadNormalMap("..\\Data\\Textures\\toy_box_normal.bmp");
	texture.loadParallaxMap("..\\Data\\Textures\\toy_box_disp.bmp");
	render[0].setTextureInfo(texture);

	RenderEngine::AddRenderable(&render[0]);

	//Cube representing light source
	render[1].setMesh(cube);
	render[1].m_vertexShaderInfo.programID = pTShaderProgramID;
	render[1].m_vertexShaderInfo.callback = uniformCallback;
	pos = render[1].getTransformInfo();
	pos.m_translateTransform = lightPosition;
	render[1].setTransformInfo(pos);
	texture = render[1].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\Stormtrooper.bmp");
	render[1].setTextureInfo(texture);
	
	RenderEngine::AddRenderable(&render[1]);

	//Exploding Clone
	render[2].setMesh(clone);
	render[2].m_vertexShaderInfo.programID = explodeShaderProgramID;
	render[2].m_vertexShaderInfo.callback = uniformCallbackExplode;
	pos = render[2].getTransformInfo();
	pos.m_translateTransform = glm::vec3(0.0f, 0.0f, 3.0f);
	render[2].setTransformInfo(pos);
	texture = render[2].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\Clonetrooper.bmp");
	render[2].setTextureInfo(texture);

	RenderEngine::AddRenderable(&render[2]);

	//Water
	render[3].setMesh(water);
	render[3].m_vertexShaderInfo.programID = waterShaderProgramID;
	render[3].m_vertexShaderInfo.callback = uniformCallbackWater;
	pos = render[3].getTransformInfo();
	pos.m_translateTransform = glm::vec3(0.0f, 8.0f, 70.0f);
	pos.m_scaleTransform = glm::vec3(3.0f, 3.0f, 3.0f);
	render[3].setTransformInfo(pos);
	texture = render[3].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\Water.bmp");
	render[3].setTextureInfo(texture);

	//RenderEngine::AddRenderable(&render[3]);

	//Skybox Faces
	const GLchar* faces[6];
	faces[0] = "..\\Data\\Textures\\totality_rt.bmp";
	faces[1] = "..\\Data\\Textures\\totality_lf.bmp";
	faces[2] = "..\\Data\\Textures\\totality_dn.bmp";
	faces[3] = "..\\Data\\Textures\\totality_up.bmp";
	faces[4] = "..\\Data\\Textures\\totality_bk.bmp";
	faces[5] = "..\\Data\\Textures\\totality_ft.bmp";

	//Skybox
	render[4].setMesh(skybox);
	render[4].m_vertexShaderInfo.programID = skyboxShaderProgramID;
	render[4].m_vertexShaderInfo.callback = uniformCallbackSkybox;
	texture = render[4].getTextureInfo();
	texture.loadCubeMap(faces);
	texture.isCubeMap = true;
	render[4].setTextureInfo(texture);

	RenderEngine::AddRenderable(&render[4]);

	refractCubeMap = texture.textureID;
	
	//Stealth Trooper
	render[5].setMesh(stealth);
	render[5].m_vertexShaderInfo.programID = environmentRefactionShaderProgramID;
	render[5].m_vertexShaderInfo.callback = uniformCallbackRefraction;
	pos = render[5].getTransformInfo();
	pos.m_translateTransform = stealthPos;
	//pos.rotatex = 1.0f;
	//pos.theta = 0.001f;
	//pos.isRotating = true;
	render[5].setTransformInfo(pos);
	texture = render[5].getTextureInfo();
	texture.textureID = refractCubeMap;
	texture.isCubeMap = true;
	render[5].setTextureInfo(texture);
	render[5].drawWithFrameBuffer = false;

	//RenderEngine::AddRenderable(&render[5]);
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