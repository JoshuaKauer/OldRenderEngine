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
bool pressed = false;

float dt = 1/60;

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

GLuint passThroughProgramID, pTShaderProgramID, pCshaderProgramID, pCTShaderProgramID, pCNShaderProgramID;

glm::vec3 cubeTrans;

glm::mat4 modelToProjectionMatrix;
glm::mat4 worldToProjectionMatrix;

void uniformCallback(Renderable*);
void uniformCallbackLight(Renderable*);

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

	SendDataToOpenGL();

	player.AddComponent(&space, "Spatial");
	player.AddComponent(&move, "Move");
	player.AddComponent(&keys, "Keys");
	player.AddComponent(&mouse, "Mouse");
	player.AddComponent(&camera, "Camera");
	player.Init();

	gTime.Initialize();

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
	cube = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\Cube.scene");

	Geometry* scene = ShapeGenerator::makeScene("..\\..\\Static_Data\\Scenes\\AIWorld.scene");

	render[0].setMesh(scene);
	render[0].m_vertexShaderInfo.programID = pTShaderProgramID;
	render[0].m_vertexShaderInfo.callback = uniformCallback;
	texture = render[0].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\Wall.bmp");
	render[0].setTextureInfo(texture);

	render[1].setMesh(cube);
	render[1].m_vertexShaderInfo.programID = pTShaderProgramID;
	render[1].m_vertexShaderInfo.callback = uniformCallback;
	texture = render[1].getTextureInfo();
	texture.loadBMP_custom("..\\Data\\Textures\\Leaf.bmp");
	render[1].setTextureInfo(texture);
	pos = render[1].getTransformInfo();
	pos.m_translateTransform = glm::vec3(0.0f, 2.0f, 5.0f);
	render[1].setTransformInfo(pos);

	RenderEngine::AddRenderable(&render[0]);
	RenderEngine::AddRenderable(&render[1]);
}

MyModel* MyGame::GetModel()
{
	return &model;
}

void uniformCallback(Renderable* randy)
{
	GLuint uniLoc = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "modelToProjectionMatrix");

	glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	glm::mat4 finalTest = worldToProjectionMatrix * randy->getTransformInfo().getFinalTransform();

	glUniformMatrix4fv(uniLoc, 1, GL_FALSE, &finalTest[0][0]);
}

void uniformCallbackLight(Renderable* randy)
{
	GLuint uniLoc = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "modelToProjectionMatrix");
	GLuint modelToWorldLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "modelToWorldMatrix");

	GLuint ambientLightUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "ambientLight");
	GLuint diffuseLightUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "lightPositionWorld");
	GLuint specularLightUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "eyePositionWorld");
	GLuint specularExpUniformLocation = glGetUniformLocation(randy->m_vertexShaderInfo.programID, "specularExp");

	glm::mat4 viewToProjectionMatrix = glm::perspective(glm::radians(45.0f), ((float)m_window->width()) / m_window->height(), 0.1f, FarPlane);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	glm::mat4 finalTest = worldToProjectionMatrix * randy->getTransformInfo().getFinalTransform();
	glm::mat4 modelToWorld = viewToProjectionMatrix * worldToViewMatrix;

	glUniformMatrix4fv(uniLoc, 1, GL_FALSE, &finalTest[0][0]);
	glUniformMatrix4fv(modelToWorldLocation, 1, GL_FALSE, &modelToWorld[0][0]);

	glm::vec4 ambientLight = glm::vec4(0.2f, 0.2f, 0.2f, 0.0f);
	glm::vec3 lightPositionWorld = glm::vec3(0.0f, 25.0f, 0.0f);
	glm::vec3 eyePosition = camera.getPosition();
	float specularExp = 50000.0f;

	glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	glUniform3fv(diffuseLightUniformLocation, 1, &lightPositionWorld[0]);
	glUniform3fv(specularLightUniformLocation, 1, &eyePosition[0]);
	glUniform1f(specularExpUniformLocation, specularExp);
}
