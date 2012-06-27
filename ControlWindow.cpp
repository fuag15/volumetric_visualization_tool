#include "ControlWindow.h"


ControlWindow::ControlWindow(QWidget * parent):QWidget(parent)
{
	cmapper = new ColorMapper(this);
	cmapbar = new ColorMapperBar(this);
	current = 0;
	colorPicker = new QColorDialog();
	colorPicker -> setOptions(QColorDialog::NoButtons); //| QColorDialog::ShowAlphaChannel);
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
	layout->addWidget(colorPicker);
	layout->setStretch(2, 1);
	layout->setStretch(0, 1);
	layout->setStretch(3, 1);
	
	setLayout(layout);
	
	cmapButton -> hide();
	
	resize(600, 800);
	
	return;
}

void ControlWindow::setAlphaArray(float* array)
{
	cmapper->setAlphaMap(array);
}

void ControlWindow::sendMaxRange(int value)
{
	emit(maxRange(value));
	emit(needUpdate());
}

void ControlWindow::fileOpened(float* fileData, int xDim, int yDim, int zDim, float maxValue)
{
	hist->openFileHist(fileData, xDim, yDim, zDim, maxValue);
}

void ControlWindow::sendColor()
{
	if(current == 0)
	{
		return;
	}
	QColor color = colorPicker->currentColor();
	current->setQColor(color);
}

void ControlWindow::remapAll()
{
	ColorMapItr* itr;
	itr = maplist->getIter();
	itr -> reset();
	while(!itr->end())
	{
		disconnect(itr->getCursor(), SIGNAL(active(MapNode*)), this, SLOT(activateMapNode(MapNode*)));
		connect(itr->getCursor(), SIGNAL(active(MapNode*)), this, SLOT(activateMapNode(MapNode*)));
		itr->advance();
	}
}

void ControlWindow::activateMapNode(MapNode* mn)
{
	if(current != 0)
	{
		disconnect(colorPicker, SIGNAL(currentColorChanged(const QColor&)), current, SLOT(uColor(const QColor&)));
	}
	current = mn;
	QColor color;
	color.setRedF(current->getRed());
	color.setGreenF(current->getGreen());
	color.setBlueF(current->getBlue());
	colorPicker->setCurrentColor(color);
	connect(colorPicker, SIGNAL(currentColorChanged(const QColor&)), current, SLOT(uColor(const QColor&)));
}

void ControlWindow::setSmap(ColorMap* smap)
{
	smap = 0;
	return;
}

void ControlWindow::breakConnection()
{
	if(current != 0)
	{
		disconnect(colorPicker, SIGNAL(currentColorChanged(const QColor&)), current, SLOT(uColor(const QColor&)));
	}
	current = 0;
}

void ControlWindow::setCmap(ColorMap* cmapr)
{
	maplist = cmapr;
	connect(cmapr, SIGNAL(sizeChanged()), this, SLOT(remapAll()));
	cmapper->setCmap(cmapr);
	cmapbar->setCmap(cmapr);
	connect(cmapr, SIGNAL(changed()), cmapbar, SLOT(updateGL()));
	connect(cmapr, SIGNAL(changed()), cmapper, SLOT(updateGL()));
	connect(cmapButton, SIGNAL(clicked()), this, SLOT(breakConnection()));
	connect(cmapButton, SIGNAL(clicked()), cmapbar, SLOT(removeCurrent()));
	cmapButton -> show();
	return;
}