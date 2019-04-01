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

GLuint depthMapFBO;
GLuint depthCubemap;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
glm::mat4 shadowTransforms[6];

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

GLuint passThroughProgramID, pTShaderProgramID, pCshaderProgramID, pCTShaderProgramID, pCNShaderProgramID, pTAlphaShaderProgramID, explodeShaderProgramID, waterShaderProgramID, skyboxShaderProgramID, environmentRefactionShaderProgramID, shadowShaderProgramID, depthShaderProgramID;

glm::vec3 cubeTrans;

glm::vec3 lightPosition = glm::vec3(-3.0f, 27.0f, -2.0f);

glm::mat4 modelToProjectionMatrix;
glm::mat4 worldToProjectionMatrix;

void uniformCallback(Renderable*);
void uniformCallbackExplode(Renderable*);
void uniformCallbackLight(Renderable*);
void uniformCallbackWater(Renderable*);
void uniformCallbackSkybox(Renderable*);
void uniformCallbackRefraction(Renderable*);
void uniformCallbackDepth(Renderable*);
void uniformCallbackShadow(Renderable*);

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

	glGenFramebuffers(1, &depthMapFBO);
	// Create depth cubemap texture
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (GLuint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// Attach cubemap as depth map FBO's color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG(Error, "Framebuffer not complete!"); 
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, 0.1f, FarPlane);
	
	shadowTransforms[0] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms[1] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms[2] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms[3] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms[4] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms[5] = (shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	RenderEngine::DepthDraw(SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	RenderEngine::Draw();
}

void MyGame::SendDataToOpenGL()
{
	TextureInfo texture;
	TransformInfo pos;
	TransformInfo scale;
	cube = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.pt.scene");

	Geometry* scene = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\AIWorld.ptn.scene");

	Geometry* clone = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Clonetrooper.scene");

	Geometry* water = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Water.scene");

	Geometry* skybox = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.p.scene");

	Geometry* stealth = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Clonetrooper.pn.scene");

	Geometry* shadowCube = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.ptn.scene");

	render[0].setMesh(scene);
	render[0].m_vertexShaderInfo.programID = shadowShaderProgramID;
	render[0].m_vertexShaderInfo.callback = uniformCallbackShadow;
	texture = render[0].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\Wall.bmp");
	texture.shadowTextureID = &depthCubemap;
	render[0].setTextureInfo(texture);
	render[0].name = "Hideout";

	RenderEngine::AddRenderable(&render[0]);

	render[1].setMesh(cube);
	render[1].m_vertexShaderInfo.programID = pCshaderProgramID;
	render[1].m_vertexShaderInfo.callback = uniformCallback;
	pos = render[1].getTransformInfo();
	pos.m_translateTransform = lightPosition;
	render[1].setTransformInfo(pos);
	render[1].name = "LightCube";
	
	RenderEngine::AddRenderable(&render[1]);

	render[2].setMesh(clone);
	render[2].m_vertexShaderInfo.programID = explodeShaderProgramID;
	render[2].m_vertexShaderInfo.callback = uniformCallbackExplode;
	pos = render[2].getTransformInfo();
	pos.m_translateTransform = glm::vec3(0.0f, 0.0f, 3.0f);
	render[2].setTransformInfo(pos);
	texture = render[2].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\Clonetrooper.bmp");
	render[2].setTextureInfo(texture);
	render[2].name = "Clone";

	RenderEngine::AddRenderable(&render[2]);

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
	render[3].name = "Water";

	RenderEngine::AddRenderable(&render[3]);

	render[4].setMesh(stealth);
	render[4].m_vertexShaderInfo.programID = environmentRefactionShaderProgramID;
	render[4].m_vertexShaderInfo.callback = uniformCallbackRefraction;
	pos = render[4].getTransformInfo();
	pos.m_translateTransform = glm::vec3(0.0f, 20.0f, 0.0f);
	pos.rotatex = 1.0f;
	pos.theta = 0.001f;
	pos.isRotating = true;
	render[4].setTransformInfo(pos);
	render[4].name = "Refract";

	RenderEngine::AddRenderable(&render[4]);


	const GLchar* faces[6];
	faces[0] = "..\\Data\\Textures\\totality_rt.bmp";
	faces[1] = "..\\Data\\Textures\\totality_lf.bmp";
	faces[2] = "..\\Data\\Textures\\totality_dn.bmp";
	faces[3] = "..\\Data\\Textures\\totality_up.bmp";
	faces[4] = "..\\Data\\Textures\\totality_bk.bmp";
	faces[5] = "..\\Data\\Textures\\totality_ft.bmp";

	render[5].setMesh(skybox);
	render[5].m_vertexShaderInfo.programID = skyboxShaderProgramID;
	render[5].m_vertexShaderInfo.callback = uniformCallbackSkybox;
	texture = render[5].getTextureInfo();
	texture.loadCubeMap(faces);
	texture.isCubeMap = true;
	render[5].setTextureInfo(texture);
	render[5].name = "Skybox";

	RenderEngine::AddRenderable(&render[5]);
}

MyModel* MyGame::GetModel()
{
	return &model;
}

void uniformCallback(Renderable* randy)
{
	GLuint uniLoc = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "modelToProjectionMatrix");
	GLuint uniLoc2 = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "myTexture");

	glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	glm::mat4 finalTest = worldToProjectionMatrix * randy->getTransformInfo().getFinalTransform();

	glUniformMatrix4fv(uniLoc, 1, GL_FALSE, &finalTest[0][0]);
	glUniform1i(uniLoc2, 0);

	//glDepthMask(GL_TRUE);
}

void uniformCallbackExplode(Renderable* randy)
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, FarPlane);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, &projection[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, &camera.getWorldToViewMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->getTransformInfo().getFinalTransform()[0][0]);
	glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "time"), totalTime);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "texture_diffuse1"), 0);

	//glDepthMask(GL_TRUE);
}

void uniformCallbackLight(Renderable* randy)
{
	GLuint uniLoc = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "modelToProjectionMatrix");
	GLuint modelToWorldLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "modelToWorldMatrix");

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

	glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	glm::mat4 finalTest = worldToProjectionMatrix * randy->getTransformInfo().getFinalTransform();

	glUniformMatrix4fv(uniLoc, 1, GL_FALSE, &finalTest[0][0]);
	glUniformMatrix4fv(modelToWorldLocation, 1, GL_FALSE, &randy->getTransformInfo().getFinalTransform()[0][0]);

	//glDepthMask(GL_TRUE);
}

void uniformCallbackWater(Renderable* randy)
{
	GLuint uniLoc = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "modelToProjectionMatrix");

	glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	glm::mat4 finalTest = worldToProjectionMatrix * randy->getTransformInfo().getFinalTransform();

	glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "time"), waterTime);

	glUniformMatrix4fv(uniLoc, 1, GL_FALSE, &finalTest[0][0]);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "myTexture"), 0);

	//glDepthMask(GL_TRUE);
}

void uniformCallbackSkybox(Renderable* randy)
{
	glm::mat4 view = glm::mat4(glm::mat3(camera.getWorldToViewMatrix()));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, &projection[0][0]);


	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "skybox"), 0);

	//glDepthMask(GL_FALSE);
}

void uniformCallbackRefraction(Renderable* randy)
{
	glm::mat4 view = camera.getWorldToViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "cameraPos"), 1, &camera.position[0]);
}

void uniformCallbackDepth(Renderable* randy)
{
	glUniformMatrix4fv(glGetUniformLocation(depthShaderProgramID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);
	for (GLuint i = 0; i < 6; ++i)
	{
		glUniformMatrix4fv(glGetUniformLocation(depthShaderProgramID, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, &shadowTransforms[i][0][0]);
	}
	glUniform1f(glGetUniformLocation(depthShaderProgramID, "far_plane"), FarPlane);
	glUniform3fv(glGetUniformLocation(depthShaderProgramID, "lightPos"), 1, &lightPosition[0]);
}

void uniformCallbackShadow(Renderable* randy)
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glm::mat4 view = camera.getWorldToViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "model"), 1, GL_FALSE, &randy->m_transformInfo.getFinalTransform()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "view"), 1, GL_FALSE, &view[0][0]);
	// Set light uniforms
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightPos"), 1, &lightPosition[0]);
	glUniform3fv(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "viewPos"), 1, &camera.position[0]);
	
	glUniform1f(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "far_plane"), FarPlane);

	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(randy->m_vertexShaderInfo.programID, "depthMap"), 1);
}