#ifndef MYGLWINDOW
#define MYGLWINDOW
#include <gl\glew.h>
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4251)
#pragma warning(disable : 4800)
#include <qtOpenGL\qglwidget>
#pragma warning(pop)
#include <Qt/qtimer.h>
#include <Qt\qapplication.h>
struct MyModel;
class MyGame;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

class MyGlWindow : public QGLWidget
{
	Q_OBJECT

	//GLuint vertexBufferID;
	QTimer myTimer;
protected:
	void initializeGL();
	void mouseMoveEvent(QMouseEvent*);

private slots:
	void myUpdate();
	
public:
	QApplication* app;
	MyGlWindow(QApplication*, MyGame*);
	void closeProgram();
};

#endif