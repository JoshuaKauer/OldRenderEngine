#pragma once
#pragma warning(push)
#pragma warning(disable : 4127)
#include <QtGui\qwidget.h>
#pragma warning(pop)
struct MyModel;
class DebugSlider;
class MyGlWindow;

class MyWidget : public QWidget
{
	Q_OBJECT

	DebugSlider* lightXSlider;
	DebugSlider* lightYSlider;
	DebugSlider* lightZSlider;

	DebugSlider* diffuseRSlider;
	DebugSlider* diffuseGSlider;
	DebugSlider* diffuseBSlider;

	DebugSlider* ambientRSlider;
	DebugSlider* ambientGSlider;
	DebugSlider* ambientBSlider;

	DebugSlider* objectColorRSlider;
	DebugSlider* objectColorGSlider;
	DebugSlider* objectColorBSlider;

	DebugSlider* specularRSlider;
	DebugSlider* specularGSlider;
	DebugSlider* specularBSlider;
	DebugSlider* specularExpSlider;

	MyGlWindow* myGlWindow;
private slots:
	void sliderValueChanged();
public:
	MyWidget(MyGlWindow*, MyModel*);
};

