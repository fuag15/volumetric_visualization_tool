#ifndef __IsoControlWindow_h__
#define __IsoControlWindow_h__

#include <QGLWidget>
#include <math.h>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QColorDialog>
#include "ColorMap.h"
#include "HistogramWidget.h"
#include "MapNode.h"
#include <QPushButton>
#include "ColorMapItr.h"
#include "SlicesControllerWidget.h"
#include "ColorMapper.h"
#include "ColorMapperBar.h"

class IsoControlWindow : public QWidget // widget to hold other widgets
{
    Q_OBJECT
    
	signals:
		void maxRange(int value);
		void needUpdate();
    
	public slots:
		void sendMaxRange(int value);
		void fileOpened(float* fileData, int xDim, int yDim, int zDim, float maxValue);
    
  public:
    
    IsoControlWindow(QWidget * parent);
    void setCmap(ColorMap* cmapr);
    void setSmap(ColorMap* smap);
    void setImap(ColorMap* imap);
    QColorDialog *colorPicker;
    void setAlphaArray(float* array);
    HistogramWidget *hist;
    MapNode* current;
    ColorMap* maplist;
    SlicesControllerWidget* smcw;
    ColorMapper *cmapper;
    ColorMapperBar *cmapbar;
    QPushButton* cmapButton;

};

#endif