#include "IsoControlWindow.h"


IsoControlWindow::IsoControlWindow(QWidget * parent):QWidget(parent)
{
	cmapper = new ColorMapper(this);
	cmapbar = new ColorMapperBar(this);
	cmapbar -> setLimit = 1;
	current = 0;
	hist = new HistogramWidget(this);
	//QPushButton* button = new QPushButton("Send Color to Current Map");
	cmapButton = new QPushButton("Delete Current Map");
	QPushButton* zoomButton = new QPushButton("Toggle Hist Zoom");
	connect(zoomButton, SIGNAL(clicked()), hist, SLOT(toggleZoom()));
	//connect(button, SIGNAL(clicked()), this, SLOT(sendColor()));
	connect(hist, SIGNAL(maxRangeGot(int)), this, SLOT(sendMaxRange(int)));
	connect(hist, SIGNAL(upperBoundGot(float)), cmapbar, SLOT(setUpperBound(float)));
	connect(hist, SIGNAL(lowerBoundGot(float)), cmapbar, SLOT(setLowerBound(float)));
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(hist);
	layout->addWidget(zoomButton);
	layout->addWidget(cmapper);
	layout->addWidget(cmapbar);
	layout->addWidget(cmapButton);
	//layout->addWidget(button);
	layout->setStretch(2, 1);
	layout->setStretch(0, 1);
	layout->setStretch(3, 1);
	
	setLayout(layout);
	
	cmapButton -> hide();
	
	resize(600, 800);
	
	return;
}

void IsoControlWindow::setAlphaArray(float* array)
{
	cmapper->setAlphaMap(array);
}

void IsoControlWindow::sendMaxRange(int value)
{
	emit(maxRange(value));
	emit(needUpdate());
}

void IsoControlWindow::fileOpened(float* fileData, int xDim, int yDim, int zDim, float maxValue)
{
	hist->openFileHist(fileData, xDim, yDim, zDim, maxValue);
}

void IsoControlWindow::setSmap(ColorMap* smap)
{
	smap = 0;
	return;
}

void IsoControlWindow::setCmap(ColorMap* cmapr)
{
	maplist = cmapr;
	cmapper->setCmap(cmapr);
	//cmapbar->setCmap(cmapr);
	//connect(cmapr, SIGNAL(changed()), cmapbar, SLOT(updateGL()));
	connect(cmapr, SIGNAL(changed()), cmapper, SLOT(updateGL()));
	//connect(cmapButton, SIGNAL(clicked()), cmapbar, SLOT(removeCurrent()));
	cmapButton -> show();
	return;
}

void IsoControlWindow::setImap(ColorMap* cmapr)
{
	maplist = cmapr;
	//cmapper->setCmap(cmapr);
	cmapbar->setCmap(cmapr);
	connect(cmapr, SIGNAL(changed()), cmapbar, SLOT(updateGL()));
	connect(cmapr, SIGNAL(changed()), cmapper, SLOT(updateGL()));
	connect(cmapButton, SIGNAL(clicked()), cmapbar, SLOT(removeCurrent()));
	cmapButton -> show();
	return;
}