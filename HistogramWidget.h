#ifndef __HistogramWidget_h__
#define __HistogramWidget_h__

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "Histogram.h"

class HistogramWidget : public QWidget // widget to hold other widgets
{
    Q_OBJECT
    
	signals:
		void maxRangeGot(int value);
		void upperBoundGot(float value);
		void lowerBoundGot(float value);
		
	public slots:
		void sendRangeOut(int value);
		void supperBoundGot(float value);
		void slowerBoundGot(float value);
		void toggleZoom();
    
  public:
    
    HistogramWidget(QWidget * parent);
    void openFileHist(float* fileData, int xDim, int yDim, int zDim, float maxValue);
    Histogram *hist;

};

#endif