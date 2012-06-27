#include "vectorWindow.h"


vectorWindow::vectorWindow(QWidget * parent):QWidget(parent)
{
	lgroup = new LicSliderGroup(this);
	ggroup = new GlyphSliderGroup(this);
	QScrollArea* sarea = new QScrollArea();
	sarea->setWidgetResizable(true);
	sarea-> setWidget(lgroup);
	QScrollArea* sarea2 = new QScrollArea();
	sarea2->setWidgetResizable(true);
	sarea2-> setWidget(ggroup);
	QVBoxLayout* layout = new QVBoxLayout();
	QPushButton* button = new QPushButton("Add New Lic Slice");
	QPushButton* button2 = new QPushButton("Add New Glyph Slice");
// 	QPushButton* buttonst = new QPushButton("Disable Slices");
// 	QPushButton* buttonit = new QPushButton("Disable IsoSurfaces");
// 	QPushButton* buttonvt = new QPushButton("Disable VolSurfaces");
// 	QPushButton* buttoncv = new QPushButton("Enable view cutAway");
// 	buttonvt -> setCheckable(1);
// 	buttonst -> setCheckable(1);
// 	buttonit -> setCheckable(1);
// 	buttoncv -> setCheckable(1);
 	connect(button, SIGNAL(clicked()), lgroup, SLOT(addSliceNode()));
	connect(button2, SIGNAL(clicked()), ggroup, SLOT(addSliceNode()));
// 	connect(buttons, SIGNAL(clicked()), this, SLOT(sendReloadOut()));
// 	connect(buttonst, SIGNAL(clicked()), this, SLOT(sendSlicesOut()));
// 	connect(buttonit, SIGNAL(clicked()), this, SLOT(sendIsosOut()));
// 	connect(buttonvt, SIGNAL(clicked()), this, SLOT(sendVolsOut()));
// 	connect(buttoncv, SIGNAL(clicked()), this, SLOT(sendCutOut()));
/*	
	
	layout -> addWidget(buttons);
	layout -> addWidget(buttonst);
	layout -> addWidget(buttonit);
	layout -> addWidget(buttonvt);
	layout -> addWidget(buttoncv);*/
	layout -> addWidget(sarea);
	layout -> addWidget(button);
	layout -> addWidget(sarea2);
	layout -> addWidget(button2);
	
	setLayout(layout);
	
	return;
}

// QSlider* SlicesControllerWidget::getDetailSlider()
// {
// 	return sgroup -> getDetail();
// }
// 
// QSlider* SlicesControllerWidget::getAlphaSlider()
// {
// 	return sgroup -> getAlpha();
// }
// void SlicesControllerWidget::sendCutOut()
// {
// 	emit(toggleCut());
// }
// 
// void SlicesControllerWidget::sendSlicesOut()
// {
// 	emit(toggleSlices());
// }
// void SlicesControllerWidget::sendIsosOut()
// {
// 	emit(toggleIsos());
// }
// void SlicesControllerWidget::sendVolsOut()
// {
// 	emit(toggleVols());
// }
// 
// void SlicesControllerWidget::sendReloadOut()
// {
// 	emit(reloadFrag());
// }

void vectorWindow::setGlyphSliceMap(ColorMap* smap)
{
 	ggroup->setSlicesMapList(smap);
	return;
}
void vectorWindow::setLicSliceMap(ColorMap* smap)
{
	lgroup->setSlicesMapList(smap);
	return;
}