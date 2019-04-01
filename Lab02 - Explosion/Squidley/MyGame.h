#pragma once
#include <gl\glew.h>
#pragma warning(push)
#pragma warning(disable : 4201)
#pragma warning(disable : 4127)
#include <qtOpenGL\qglwidget>
#pragma warning(pop)
struct MyModel;
class MyGlWindow;
class MyGame
{
public:
	MyGame();

	bool Initialize(MyGlWindow*);

	bool Shutdown();

	void Update();

	void ProcessKeys();

	void ProcessMouse(QMouseEvent*);

	void Draw();

	void SendDataToOpenGL();

	MyModel* GetModel();					
};

