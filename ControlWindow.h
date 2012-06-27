#ifndef __ControlWindow_h__
#define __ControlWindow_h__

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

class ControlWindow : public QWidget // widget to hold other widgets
{
    Q_OBJECT
    
	signals:
		void maxRange(int value);
		void needUpdate();
    
	public slots:
		void breakConnection();
		void sendMaxRange(int value);
		void activateMapNode(MapNode* mn);
		void remapAll();
		void sendColor();
		void fileOpened(float* fileData, int xDim, int yDim, int zDim, float maxValue);
    
  public:
    
    ControlWindow(QWidget * parent);
    void setCmap(ColorMap* cmapr);
    void setSmap(ColorMap* smap);
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