#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4201)
#include <QtGui/qmouseevent>
#pragma warning (pop)
#include "MyGlWindow.h"
#include "MyGame.h"

MyGame* m_game;

void MyGlWindow::initializeGL()
{
	setMouseTracking(true);
	m_game->Initialize(this);
	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(0);
}

void MyGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	m_game->ProcessMouse(e);
}

void MyGlWindow::myUpdate()
{
	//transformShape();
	m_game->Update();
	repaint();
}

void MyGlWindow::closeProgram()
{
	app->exit();
}

MyGlWindow::MyGlWindow(QApplication* _app, MyGame* game) :
app(_app)
{
	m_game = game;
	setMinimumSize(SCREEN_WIDTH, SCREEN_HEIGHT);
}
