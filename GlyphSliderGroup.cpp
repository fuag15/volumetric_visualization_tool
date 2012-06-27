#include "GlyphSliderGroup.h"

GlyphSliderGroup::GlyphSliderGroup(QWidget * parent):QWidget(parent)
{
	layout = new QVBoxLayout();
	setLayout(layout);
// 	QLabel* label2 = new QLabel("Step Detail for Iso Surface And Vol Rendering");
// 	QLabel* label3 = new QLabel("Alpha correction adjustment");
	QLabel* label = new QLabel("Glyph Map Positions");
// 	layout->addWidget(label2);
// 	detail = new QSlider();
// 	detail -> setMaximum(256);
// 	detail -> setMinimum(1);
// 	detail -> setOrientation(Qt::Horizontal);
// 	layout->addWidget(detail);
// // 	layout->addWidget(label3);
// 	alpha = new QSlider();
// 	alpha -> setMaximum(256);
// 	alpha -> setMinimum(1);
// 	alpha -> setOrientation(Qt::Horizontal);
// 	layout -> addWidget(alpha);
	layout->addWidget(label);
	return;
}

QSlider* GlyphSliderGroup::getDetail()
{
	return detail;
}

QSlider* GlyphSliderGroup::getAlpha()
{
	return alpha;
}


void GlyphSliderGroup::setSlicesMapList(ColorMap* cmaplistin)
{
	cmaplist = cmaplistin;
	
	return;
}

void GlyphSliderGroup::addSliceNode()
{
	addSlicesMap(0,0,1,1,1);
}

void GlyphSliderGroup::addSlicesMap(int x, int y, int z, int alpha, int value)
{
	QVBoxLayout* tempOutLayout = new QVBoxLayout();
	QHBoxLayout* tempLayout = new QHBoxLayout();
	QPushButton* button = new QPushButton("Remove");
	QPushButton* buttonx = new QPushButton("X");
	QPushButton* buttony = new QPushButton("Y");
	QPushButton* buttonz = new QPushButton("Z");
	QPushButton* buttonv = new QPushButton("V");
	QPushButton* buttont = new QPushButton("Toggle");
	
	button -> setCheckable(1);
	buttonx -> setCheckable(1);
	buttony -> setCheckable(1);
	buttonz -> setCheckable(1);
	buttonv -> setCheckable(1);
	buttont -> setCheckable(1);
	buttonz -> setChecked(1);
	
	MapNode* current;
	QSlider* slider = new QSlider();
	current = cmaplist -> addMap(x, y, z, alpha, value);
	current -> setButtons(buttonx, buttony, buttonz, buttonv);
	slider -> setMaximum(256);
	slider -> setMinimum(1);
	slider -> setOrientation(Qt::Horizontal);
	connect(slider, SIGNAL(valueChanged(int)), current, SLOT(uValue(int)));
	connect(slider, SIGNAL(sliderPressed()), current, SLOT(setActive()));
	connect(button, SIGNAL(clicked()), button, SLOT(close()));
	connect(button, SIGNAL(clicked()), buttonx, SLOT(close()));
	connect(button, SIGNAL(clicked()), buttony, SLOT(close()));
	connect(button, SIGNAL(clicked()), buttonz, SLOT(close()));
	connect(button, SIGNAL(clicked()), buttonv, SLOT(close()));
	connect(button, SIGNAL(clicked()), buttont, SLOT(close()));
	connect(button, SIGNAL(clicked()), slider, SLOT(close()));
	
	
	connect(button, SIGNAL(clicked()), current, SLOT(selfDestruct()));
	connect(buttonx, SIGNAL(clicked()), current, SLOT(setX()));
	connect(buttony, SIGNAL(clicked()), current, SLOT(setY()));
	connect(buttonz, SIGNAL(clicked()), current, SLOT(setZ()));
	connect(buttonv, SIGNAL(clicked()), current, SLOT(setV()));
	connect(buttont, SIGNAL(clicked()), current, SLOT(setToggle()));
	
	connect(current, SIGNAL(closeMe()), button, SLOT(close()));
	connect(current, SIGNAL(closeMe()), buttonx, SLOT(close()));
	connect(current, SIGNAL(closeMe()), buttony, SLOT(close()));
	connect(current, SIGNAL(closeMe()), buttonz, SLOT(close()));
	connect(current, SIGNAL(closeMe()), buttonv, SLOT(close()));
	connect(current, SIGNAL(closeMe()), buttont, SLOT(close()));
	connect(current, SIGNAL(closeMe()), slider, SLOT(close()));
	
	//tempLayout->addWidget(slider);
	tempOutLayout->addWidget(slider);
	
	tempLayout->addWidget(buttonx);
	tempLayout->addWidget(buttony);
	tempLayout->addWidget(buttonz);
	tempLayout->addWidget(buttonv);
	tempLayout->addWidget(buttont);
	tempLayout->addWidget(button);
	tempOutLayout->addLayout(tempLayout);
	
	layout->addLayout(tempOutLayout);
	
	return;
}