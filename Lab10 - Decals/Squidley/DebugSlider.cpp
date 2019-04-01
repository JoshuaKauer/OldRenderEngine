#pragma warning(push)
#pragma warning(disable : 4127)
#pragma warning(disable : 4201)
#pragma warning(disable : 4251)
#include "DebugSlider.h"
#include <QtGui\qvboxlayout>
#include <QtGui\qslider>
#include <QtGui\qlabel>
#pragma warning(pop)
DebugSlider::DebugSlider(float value, float min, float max, bool textOnLeft, float granularity)
{
	QLayout* layout;
	this->min = min;

	this->max = max;
	sliderGranularity = granularity;
	setLayout(layout = textOnLeft ? (QLayout*)new QHBoxLayout : new QVBoxLayout);
	layout->addWidget(label = new QLabel);
	label->setMinimumWidth(35);
	layout->addWidget(slider = new QSlider);
	label->setAlignment(Qt::AlignCenter);
	slider->setOrientation(Qt::Horizontal);
	slider->setMinimum(0);
	slider->setMaximum((int)sliderGranularity);
	connect(slider, SIGNAL(valueChanged(int)),
		this, SLOT(sliderValueChanged()));
	sliderValueChanged();
	setValue(value);
}

float DebugSlider::value() const
{
	return min + (max - min) * (slider->value() / sliderGranularity);
}

void DebugSlider::setValue(float newValue)
{
	float spot = (newValue - min) / (max - min);
	slider->setValue((int)(spot * sliderGranularity));
}

void DebugSlider::sliderValueChanged()
{
	label->setText(QString::number(value()));
	emit valueChanged(value());
}
