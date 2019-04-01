#include "MyGlWindow.h"
#include <Qt\qapplication.h>
#include "MyWidget.h"
#include "MyGame.h"
#include "Logger.h"
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MyGame game;
	Logger::initialize("..\\Data\\Logger.html");
	MyGlWindow* window = new MyGlWindow(&app, &game);
	MyWidget widg(window, game.GetModel());
	widg.show();
	//widg.showFullScreen();
	Logger::shutDown();
	return app.exec();
} 