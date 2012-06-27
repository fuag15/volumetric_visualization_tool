#include "HistogramWidget.h"

HistogramWidget::HistogramWidget(QWidget * parent) : QWidget(parent)
{
	QLabel *topLeft = new QLabel("20");
	QLabel *botLeft = new QLabel("0");
	QLabel *botRight = new QLabel("256");
	
	//topLeft -> setAlignment(Qt::AlignTop);
	//botLeft -> setAlignment(Qt::AlignRight);
	//botRight -> setAlignment(Qt::AlignRight);
	
	topLeft -> setAlignment(Qt::AlignLeft | Qt::AlignBottom);
	botLeft->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	botRight -> setAlignment(Qt::AlignRight | Qt::AlignTop);
	
	hist = new Histogram(this);
	connect(hist, SIGNAL(updatedMaxIntensity(int)), topLeft, SLOT(setNum(int)));
	connect(hist, SIGNAL(updatedMinRange(int)), botLeft, SLOT(setNum(int)));
	connect(hist, SIGNAL(updatedMaxRange(int)), botRight, SLOT(setNum(int)));
	connect(hist, SIGNAL(updatedMaxRange(int)), this, SLOT(sendRangeOut(int)));
	connect(hist, SIGNAL(setUpRange(float)), this, SLOT(supperBoundGot(float)));
	connect(hist, SIGNAL(setDownRange(float)), this, SLOT(slowerBoundGot(float)));
	connect(hist, SIGNAL(updatedMaxRangeInternal(int)), botRight, SLOT(setNum(int)));
	
	//connect(slider, SIGNAL(valueChanged(int)), viewer, SLOT(setIter(int)));
	
	QVBoxLayout *layout = new QVBoxLayout;
	layout -> addWidget(topLeft);
	layout -> addWidget(hist);
	QHBoxLayout *layout2 = new QHBoxLayout;
	layout2 -> addWidget(botLeft);
	layout2 -> addWidget(botRight);
	layout -> addLayout(layout2);
	/*QGridLayout *layout = new QGridLayout;
	layout->setHorizontalSpacing(2);
	layout->setVerticalSpacing(2);
	layout->addWidget(hist, 0, 1);
	layout->addWidget(topLeft, 0, 0);
	layout->addWidget(botLeft, 1, 0);
	layout->addWidget(botRight, 1, 1);
	layout->setColumnStretch(1, 1);
	layout->setRowStretch(0, 1);*/
	//layout->addWidget(hist2);
	//layout->addWidget(colorPicker);
	setLayout(layout);
	return;
}

void HistogramWidget::toggleZoom()
{
	hist->toggleZoom();
}

void HistogramWidget::supperBoundGot(float value)
{
	emit(upperBoundGot(value));
}
void HistogramWidget::slowerBoundGot(float value)
{
	emit(lowerBoundGot(value));
}

void HistogramWidget::sendRangeOut(int value)
{
	emit(maxRangeGot(value));
}

void HistogramWidget::openFileHist(float* fileData, int xDim, int yDim, int zDim, float maxValue)
{
	hist->readFile(fileData, xDim, yDim, zDim, maxValue);
}