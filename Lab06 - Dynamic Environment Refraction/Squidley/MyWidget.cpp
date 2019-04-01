#include "MyWidget.h"
#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4251)
#include <Qt\qdebug.h>
#pragma warning(pop)
#pragma warning(push)
#pragma warning(disable : 4251)
#include <QtGui\qvboxlayout>
#pragma warning(pop)
#include "MyGlWindow.h"
#include "DebugSlider.h"
#include "MyModel.h"

MyModel* myModel;

MyWidget::MyWidget(MyGlWindow* myWindow, MyModel* model)
{
	myModel = model;
	QVBoxLayout* mainLayout;
	setLayout(mainLayout = new QVBoxLayout);
	QVBoxLayout* controlsLayout;
	controlsLayout = new QVBoxLayout;
	//mainLayout->addLayout(controlsLayout = new QVBoxLayout);
	mainLayout->addWidget(myWindow);

	QHBoxLayout* objectColorLayout;
	controlsLayout->addLayout(objectColorLayout = new QHBoxLayout);
	objectColorLayout->addWidget(objectColorRSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));
	objectColorLayout->addWidget(objectColorGSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));
	objectColorLayout->addWidget(objectColorBSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));

	myModel->objectColor.x = 1.0f;
	myModel->objectColor.y = 1.0f;
	myModel->objectColor.z = 1.0f;

	connect(objectColorRSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(objectColorGSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(objectColorBSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));

	QHBoxLayout* ambientIntensityLayout;
	controlsLayout->addLayout(ambientIntensityLayout = new QHBoxLayout);
	ambientIntensityLayout->addWidget(ambientRSlider = new DebugSlider(0.05f, 0.0f, 1.0f, false, 40.0f));
	ambientIntensityLayout->addWidget(ambientGSlider = new DebugSlider(0.05f, 0.0f, 1.0f, false, 40.0f));
	ambientIntensityLayout->addWidget(ambientBSlider = new DebugSlider(0.05f, 0.0f, 1.0f, false, 40.0f));

	connect(ambientRSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(ambientGSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(ambientBSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));

	myModel->ambientIntensity.x = 0.05f;
	myModel->ambientIntensity.y = 0.05f;
	myModel->ambientIntensity.z = 0.05f;

	QHBoxLayout* diffuseIntensityLayout;
	controlsLayout->addLayout(diffuseIntensityLayout = new QHBoxLayout);
	diffuseIntensityLayout->addWidget(diffuseRSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));
	diffuseIntensityLayout->addWidget(diffuseGSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));
	diffuseIntensityLayout->addWidget(diffuseBSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));

	myModel->diffuseIntensity.r = 1.0f;
	myModel->diffuseIntensity.g = 1.0f;
	myModel->diffuseIntensity.b = 1.0f;

	connect(diffuseRSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(diffuseGSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(diffuseBSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));

	QHBoxLayout* lightPositionLayout;
	controlsLayout->addLayout(lightPositionLayout = new QHBoxLayout);
	lightPositionLayout->addWidget(lightXSlider = new DebugSlider);
	lightPositionLayout->addWidget(lightYSlider = new DebugSlider(3.0f));
	lightPositionLayout->addWidget(lightZSlider = new DebugSlider);

	myModel->lightPosition.x = 0.0f;
	myModel->lightPosition.y = 3.0f;
	myModel->lightPosition.z = 0.0f;

	connect(lightXSlider, SIGNAL(valueChanged(float)), 
		this, SLOT(sliderValueChanged()));
	connect(lightYSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(lightZSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));

	QHBoxLayout* specularIntensityLayout;
	controlsLayout->addLayout(specularIntensityLayout = new QHBoxLayout);
	specularIntensityLayout->addWidget(specularRSlider = new DebugSlider(0.0f, 0.0f, 1.0f, false, 40.0f));
	specularIntensityLayout->addWidget(specularGSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));
	specularIntensityLayout->addWidget(specularBSlider = new DebugSlider(1.0f, 0.0f, 1.0f, false, 40.0f));
	specularIntensityLayout->addWidget(specularExpSlider = new DebugSlider(1000.0f, 25.0f, 1000.0f, false, 39.0f));

	myModel->specularColor = glm::vec3(0.0f, 1.0f, 1.0f);
	myModel->specularExponent = 500.0f;

	connect(specularRSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(specularGSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(specularBSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
	connect(specularExpSlider, SIGNAL(valueChanged(float)),
		this, SLOT(sliderValueChanged()));
}

void MyWidget::sliderValueChanged()
{
	myModel->lightPosition.x = lightXSlider->value();
	myModel->lightPosition.y = lightYSlider->value();
	myModel->lightPosition.z = lightZSlider->value();
		   
	myModel->diffuseIntensity.r = diffuseRSlider->value();
	myModel->diffuseIntensity.g = diffuseGSlider->value();
	myModel->diffuseIntensity.b = diffuseBSlider->value();
		   
	myModel->ambientIntensity.r = ambientRSlider->value();
	myModel->ambientIntensity.g = ambientGSlider->value();
	myModel->ambientIntensity.b = ambientBSlider->value();
		   
	myModel->objectColor.r = objectColorRSlider->value();
	myModel->objectColor.g = objectColorGSlider->value();
	myModel->objectColor.b = objectColorBSlider->value();
		   
	myModel->specularColor.r = specularRSlider->value();
	myModel->specularColor.g = specularGSlider->value();
	myModel->specularColor.b = specularBSlider->value();
	myModel->specularExponent = specularExpSlider->value();
}

